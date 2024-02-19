/*
 * W5500Interface.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: Joseph Ladino
 */

#include "W5500Interface.hpp"
#include "globals.h"
#include "wizchip_conf.h"

#ifdef ETH_DEBUG
#define ETH_Printf(...) USB_Printf(__VA_ARGS__)
#else
#define ETH_Printf(...)
#endif

//void ETH_Printf(const char * fmt, ...) {
//#ifdef ETH_Debug
//	USB_Printf(fmt, ...args);
//#endif
//}

namespace JOELIB {

W5500_Interface::W5500_Interface() {
}

W5500_Interface& W5500_Interface::instance() {
	static W5500_Interface INSTANCE;
	return INSTANCE;
}

void W5500_Interface::enableChipSelect() {
	auto &inst = instance();
	HAL_GPIO_WritePin(inst.chipSelectPort, inst.chipSelectPin, GPIO_PIN_RESET);
}

void W5500_Interface::disableChipSelect() {
	auto &inst = instance();
	HAL_GPIO_WritePin(inst.chipSelectPort, inst.chipSelectPin, GPIO_PIN_SET);
}

void W5500_Interface::readBuffer(uint8_t *buf, uint16_t len) {
	auto &inst = instance();
	while (HAL_SPI_GetState(inst.spi) != HAL_SPI_STATE_READY)
		;
	auto res = HAL_SPI_Receive(inst.spi, buf, len, 50);
	if (res != HAL_OK) {
		ETH_Printf("Error reading from buffer, %x\n", res);
	}
}

void W5500_Interface::writeBuffer(uint8_t *buf, uint16_t len) {
	auto &inst = instance();
	while (HAL_SPI_GetState(inst.spi) != HAL_SPI_STATE_READY)
		;
	auto res = HAL_SPI_Transmit(inst.spi, buf, len, 50);
	if (res != HAL_OK) {
		ETH_Printf("Error writing to buffer, %x\n", res);
	}
}

uint8_t W5500_Interface::readByte() {
	uint8_t byte;
	readBuffer(&byte, sizeof(byte));
	return byte;
}

void W5500_Interface::writeByte(uint8_t byte) {
	writeBuffer(&byte, sizeof(byte));
}

void W5500_Interface::CB_DHCP_IPAssigned() {
	instance().ipAssigned = true;
	ETH_Printf("Callback: IP assigned! Leased time: %ul sec\n",
			getDHCPLeasetime());
}

void W5500_Interface::CB_DHCP_IPConflict() {
	ETH_Printf("Callback: IP conflict!\n");
}

void W5500_Interface::triggerReset() {
	HAL_GPIO_WritePin(resetPort, resetPin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(resetPort, resetPin, GPIO_PIN_SET);
	HAL_Delay(5);
}

W5500_Interface& W5500_Interface::init(SPI_HandleTypeDef *spi,
		GPIO_TypeDef *chipSelectPort, uint16_t chipSelectPin,
		GPIO_TypeDef *resetPort, uint16_t resetPin) {
	ETH_Printf("\ninit() called!\n");


	auto &inst = instance();

	inst.spi = spi;
	inst.chipSelectPort = chipSelectPort;
	inst.chipSelectPin = chipSelectPin;
	inst.resetPort = resetPort;
	inst.resetPin = resetPin;

	intr_kind intrTmp = IK_DEST_UNREACH;

	inst.triggerReset();
	HAL_Delay(500);

	auto &netInfo = inst.netInfo = wiz_NetInfo { .mac = { 0xEA, 0x11, 0x22,
			0x33, 0x44, 0xEA }, .ip = { 192, 168, 7, 135 }, .sn = { 255, 255,
			255, 0 }, .gw = { 192, 168, 7, 1 }, .dns = { 192, 168, 7, 1 },
			.dhcp = NETINFO_STATIC };
	auto &ipAssigned = inst.ipAssigned;



	ETH_Printf("Registering W5500 callbacks...\n");
	reg_wizchip_cs_cbfunc(enableChipSelect, disableChipSelect);
	reg_wizchip_spi_cbfunc(readByte, writeByte);
	reg_wizchip_spiburst_cbfunc(readBuffer, writeBuffer);

	ETH_Printf("Calling wizchip_init()...\n");
	uint8_t rx_tx_buff_sizes[2][8] = {{ 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2, 2, 2, 2, 2, 2 }};
	if(ctlwizchip(CW_INIT_WIZCHIP, (void*)rx_tx_buff_sizes) == -1) {
		ETH_Printf("W5x00 initialized fail.\n");
	}

	if(ctlwizchip(CW_SET_INTRMASK, &intrTmp) == -1) {
		ETH_Printf("W5x00 interrupt\n");
	}

	wizchip_setnetinfo(&netInfo);

	ETH_Printf("Calling DHCP_init()...\n");
//	setSHAR(netInfo.mac);
	DHCP_init(0, (uint8_t *)inst.DHCPBuffer);

	ETH_Printf("Registering DHCP callbacks...\n");
	reg_dhcp_cbfunc(CB_DHCP_IPAssigned, CB_DHCP_IPAssigned, CB_DHCP_IPConflict);

	ETH_Printf("Calling DHCP_run()...\n");

	uint8_t dhcpRet = 0;
	uint16_t dhcpTries = 0;

	while (dhcpTries < 10) {
		int i = 0;
		while(i++ < 10) {
			dhcpRet = DHCP_run();

			if (dhcpRet == DHCP_FAILED)
				dhcpTries++;
			else if (dhcpRet == DHCP_IP_LEASED)
				break;
		}
		if(dhcpRet == DHCP_IP_LEASED) break;
		DHCP_stop();
		dhcpTries++;
		HAL_Delay(10);
		DHCP_init(0, (uint8_t *)inst.DHCPBuffer);
		HAL_Delay(1000);
	}

	if (!ipAssigned) {
		ETH_Printf("\nIP was not assigned :(\n");
		return inst;
	}

	getIPfromDHCP(netInfo.ip);
	getGWfromDHCP(netInfo.gw);
	getSNfromDHCP(netInfo.sn);

	uint8_t dns[4];
	getDNSfromDHCP(dns);

	ETH_Printf(
			"IP:  %d.%d.%d.%d\nGW:  %d.%d.%d.%d\nNet: %d.%d.%d.%d\nDNS: %d.%d.%d.%d\n",
			netInfo.ip[0], netInfo.ip[1], netInfo.ip[2], netInfo.ip[3],
			netInfo.gw[0], netInfo.gw[1], netInfo.gw[2], netInfo.gw[3],
			netInfo.sn[0], netInfo.sn[1], netInfo.sn[2], netInfo.sn[3], dns[0],
			dns[1], dns[2], dns[3]);

	return inst;
}

} /* namespace JOELIB */
