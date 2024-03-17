/*
 * W5500Interface.cpp
 *
 *  Created on: Feb 17, 2024
 *      Author: Joseph Ladino
 */

#include <cstring>

#include "W5500Interface.hpp"
#include "W5500Socket.hpp"
#include "CountdownTimer.hpp"
#include "globals.h"
#include "wizchip_conf.h"

//void ETH_Printf(const char * fmt, ...) {
//#ifdef ETH_Debug
//	USB_Printf(fmt, ...args);
//#endif
//}

namespace JETHERNET {

#define getInst() W5500Interface::instance()

//static W5500Interface& inst = W5500Interface::instance();

volatile uint8_t DHCPBuffer[ETH_BUFFER_SIZE * 1024];
volatile uint8_t DNSBuffer[ETH_BUFFER_SIZE * 1024];

//wiz_NetInfo getNetInfo();
//void setNetInfo();
void printNetInfo(NetConfig*);

void enableChipSelect() {
	auto &conf = W5500Interface::config;
	HAL_GPIO_WritePin(conf.chipSelectPort, conf.chipSelectPin, GPIO_PIN_RESET);
}

void disableChipSelect() {
	auto &conf = W5500Interface::config;
	HAL_GPIO_WritePin(conf.chipSelectPort, conf.chipSelectPin, GPIO_PIN_SET);
}

void readBuffer(uint8_t *buf, uint16_t len) {
	auto &conf = W5500Interface::config;
	while (HAL_SPI_GetState(conf.spi) != HAL_SPI_STATE_READY)
		;
	auto res = HAL_SPI_Receive(conf.spi, buf, len, 50);
	if (res != HAL_OK) {
		ETH_Printf("Error reading from buffer, %x\n", res);
	}
}

void writeBuffer(uint8_t *buf, uint16_t len) {
	auto &conf = W5500Interface::config;
	while (HAL_SPI_GetState(conf.spi) != HAL_SPI_STATE_READY)
		;
	auto res = HAL_SPI_Transmit(conf.spi, buf, len, 50);
	if (res != HAL_OK) {
		ETH_Printf("Error writing to buffer, %x\n", res);
	}
}

uint8_t readByte() {
	uint8_t byte;
	readBuffer(&byte, sizeof(byte));
	return byte;
}

void writeByte(uint8_t byte) {
	writeBuffer(&byte, sizeof(byte));
}

void CB_DHCP_IPAssigned() {
	getInst().ipAssigned = true;
	ETH_Printf("Callback: IP assigned! Leased time: %lu sec\n",
			getDHCPLeasetime());
}

void CB_DHCP_IPConflict() {
	ETH_Printf("Callback: IP conflict!\n");
}

W5500Interface& W5500Interface::instance() {
	static W5500Interface INSTANCE;
	return INSTANCE;
}

void W5500Interface::triggerReset() {
	HAL_GPIO_WritePin(config.resetPort, config.resetPin, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(config.resetPort, config.resetPin, GPIO_PIN_SET);
	HAL_Delay(5);
}

bool W5500Interface::initChip() {
	ETH_Printf("Calling wizchip_init()...\n");
	uint8_t rx_tx_buff_sizes[2][8] = { { 4, 4, 8, 0, 0, 0, 0, 0 }, { 4, 4, 8, 0,
			0, 0, 0, 0 } };

	if (ctlwizchip(CW_INIT_WIZCHIP, (void*) rx_tx_buff_sizes) == -1) {
		ETH_Printf("W5x00 initialized fail.\n");
		return false;
	}

	intr_kind intrTmp = IK_DEST_UNREACH;
	if (ctlwizchip(CW_SET_INTRMASK, &intrTmp) == -1) {
		ETH_Printf("W5x00 interrupt\n");
		return false;
	}

	return true;
}

bool W5500Interface::initDHCP(W5500Socket& sock, NetConfig *conf, uint16_t timeoutMs) {
	ETH_Printf("Calling DHCP_init()...\n");
	setSHAR(conf->mac.raw);
	DHCP_init(sock.getSocketNum(), (uint8_t*) DHCPBuffer);

	ETH_Printf("Calling DHCP_run()...\n");

	CountdownTimer timer(timeoutMs);
	while(!(DHCP_run() == DHCP_IP_LEASED || timer.expired())) ;

	if (!ipAssigned) {
		ETH_Printf("\nIP was not assigned :(\n");
		return false;
	}

	getIPfromDHCP(conf->ip.raw);
	getGWfromDHCP(conf->gateway.raw);
	getSNfromDHCP(conf->subnet.raw);
	getDNSfromDHCP(conf->dns.raw);

	// apply IP assigned by router
//	setNetInfo(&netInfo);
	printNetInfo(conf);

	return true;
}

bool W5500Interface::initDNS() {
	return false;
}

bool W5500Interface::init(W5500Config _config) {
	ETH_Printf("\ninit() called!\n");

	auto &inst = instance();
	inst.config = _config;

//	conf.spi = spi;
//	conf.chipSelectPort = chipSelectPort;
//	conf.chipSelectPin = chipSelectPin;
//	conf.resetPort = resetPort;
//	conf.resetPin = resetPin;

	inst.triggerReset();
	HAL_Delay(10);

	ETH_Printf("Registering W5500 callbacks...\n");
	reg_wizchip_cs_cbfunc(enableChipSelect, disableChipSelect);
	reg_wizchip_spi_cbfunc(readByte, writeByte);
	reg_wizchip_spiburst_cbfunc(readBuffer, writeBuffer);

	ETH_Printf("Registering DHCP callbacks...\n");
	reg_dhcp_cbfunc(CB_DHCP_IPAssigned, CB_DHCP_IPAssigned, CB_DHCP_IPConflict);

	return inst.initChip();

//	inst.initDHCP();
}

wiz_NetInfo getNetInfo() {
	static wiz_NetInfo netInfo;
	wizchip_getnetinfo(&netInfo);

	return netInfo;
}

void setNetInfo(wiz_NetInfo *netInfo) {
	wizchip_setnetinfo(netInfo);
}

void printNetInfo(NetConfig* netConf) {
	static char buf[153];
	netConf->cString(buf);

	ETH_Printf("\n%s\n", buf);
}

} /* namespace JOELIB */