/*
 * W5500Interface.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: Joseph Ladino
 */

#include "W5500Interface.hpp"
#include "globals.h"
#include "wizchip_conf.h"
#include "string.h"

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

void printNetInfo();

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
	ETH_Printf("Callback: IP assigned! Leased time: %lu sec\n",
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

void W5500_Interface::initChip() {
	auto &inst = instance();

	ETH_Printf("Calling wizchip_init()...\n");
	uint8_t rx_tx_buff_sizes[2][8] = { { 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2, 2,
			2, 2, 2, 2 } };
	if (ctlwizchip(CW_INIT_WIZCHIP, (void*) rx_tx_buff_sizes) == -1) {
		ETH_Printf("W5x00 initialized fail.\n");
	}

	intr_kind intrTmp = IK_DEST_UNREACH;
	if (ctlwizchip(CW_SET_INTRMASK, &intrTmp) == -1) {
		ETH_Printf("W5x00 interrupt\n");
	}

	// wait until phy is active
	int8_t phyLink = PHY_LINK_OFF;
	while (phyLink == PHY_LINK_OFF && ctlwizchip(CW_GET_PHYLINK, &phyLink) != -1)
		;

	ETH_Printf("Confirmed PHY Link Active!\n");

	auto netInfo = wiz_NetInfo { .mac = { 0x00, 0x08, 0xdc, 0x00, 0x00, 0x00 },
			.ip = { 192, 168, 7, 189 }, .sn = { 255, 255, 255, 0 }, .gw = { 192,
					168, 7, 1 }, .dns = { 192, 168, 7, 1 }, .dhcp =
					NETINFO_STATIC };

	setNetInfo(&netInfo);
}

void printNetInfo() {
	static wiz_NetInfo netInfo;
	memset(&netInfo, 0, sizeof(netInfo));

	wizchip_getnetinfo(&netInfo);
	ETH_Printf("MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n\r",
			netInfo.mac[0], netInfo.mac[1], netInfo.mac[2], netInfo.mac[3],
			netInfo.mac[4], netInfo.mac[5]);
	ETH_Printf("IP  Address : %d.%d.%d.%d\n\r", netInfo.ip[0], netInfo.ip[1],
			netInfo.ip[2], netInfo.ip[3]);
	ETH_Printf("Subnet Mask : %d.%d.%d.%d\n\r", netInfo.sn[0], netInfo.sn[1],
			netInfo.sn[2], netInfo.sn[3]);
	ETH_Printf("Gateway     : %d.%d.%d.%d\n\r", netInfo.gw[0], netInfo.gw[1],
			netInfo.gw[2], netInfo.gw[3]);
	ETH_Printf("DNS Server  : %d.%d.%d.%d\n\r", netInfo.dns[0], netInfo.dns[1],
			netInfo.dns[2], netInfo.dns[3]);
}

void W5500_Interface::initDHCP() {
	auto netInfo = getNetInfo();

	ETH_Printf("Calling DHCP_init()...\n");
	setSHAR(netInfo.mac);
	DHCP_init(0, (uint8_t*) DHCPBuffer);

	ETH_Printf("Calling DHCP_run()...\n");

	uint8_t dhcpRet = 0;
	uint16_t dhcpTries = 0;

	while (dhcpTries < 5) {
		dhcpRet = DHCP_run();
		if (dhcpRet == DHCP_FAILED)
			dhcpTries++;
		else if (dhcpRet == DHCP_IP_LEASED)
			break;
	}

	if (!ipAssigned) {
		ETH_Printf("\nIP was not assigned :(\n");
		return;
	}

	getIPfromDHCP(netInfo.ip);
	getGWfromDHCP(netInfo.gw);
	getSNfromDHCP(netInfo.sn);
	getDNSfromDHCP(netInfo.dns);

	// apply IP assigned by router
	setNetInfo(&netInfo);
	printNetInfo();
}

void W5500_Interface::init(SPI_HandleTypeDef *spi, GPIO_TypeDef *chipSelectPort,
		uint16_t chipSelectPin, GPIO_TypeDef *resetPort, uint16_t resetPin) {
	ETH_Printf("\ninit() called!\n");

	auto &inst = instance();

	inst.spi = spi;
	inst.chipSelectPort = chipSelectPort;
	inst.chipSelectPin = chipSelectPin;
	inst.resetPort = resetPort;
	inst.resetPin = resetPin;

	inst.triggerReset();
	HAL_Delay(500);

	ETH_Printf("Registering W5500 callbacks...\n");
	reg_wizchip_cs_cbfunc(enableChipSelect, disableChipSelect);
	reg_wizchip_spi_cbfunc(readByte, writeByte);
	reg_wizchip_spiburst_cbfunc(readBuffer, writeBuffer);

	inst.initChip();

	ETH_Printf("Registering DHCP callbacks...\n");
	reg_dhcp_cbfunc(CB_DHCP_IPAssigned, CB_DHCP_IPAssigned, CB_DHCP_IPConflict);

	inst.initDHCP();
}

wiz_NetInfo W5500_Interface::getNetInfo() {
	static wiz_NetInfo netInfo;
	wizchip_getnetinfo(&netInfo);

	return netInfo;
}

void W5500_Interface::setNetInfo(wiz_NetInfo *netInfo) {
	wizchip_setnetinfo(netInfo);
}

} /* namespace JOELIB */
