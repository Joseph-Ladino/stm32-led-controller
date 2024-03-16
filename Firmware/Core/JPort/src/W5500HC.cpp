/*
 * W500HC.cpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#include <W5500HC.hpp>
#include <cstring>
#include "wizchip_conf.h"
#include "dhcp.h"
#include "W5500Socket.hpp"
#include <stdint.h>

namespace JETHERNET {

#define MemcopyConfig(member1, member2) memcpy(member1, member2, (sizeof(member1) < sizeof(member2)) ? sizeof(member1) : sizeof(member2))

void W5500HC::setConfig(NetConfig &netConfig) {
//	wiz_NetInfo wNetInfo = { 0 };
//	MemcopyConfig(wNetInfo.ip, netConfig.ip.raw);
//	MemcopyConfig(wNetInfo.sn, netConfig.subnet.raw);
//	MemcopyConfig(wNetInfo.gw, netConfig.gateway.raw);
//	MemcopyConfig(wNetInfo.dns, netConfig.dns.raw);
//	MemcopyConfig(wNetInfo.mac, netConfig.mac.raw);
//
//	wizchip_setnetinfo(&wNetInfo);

	setSHAR(netConfig.mac.raw);
	setGAR(netConfig.gateway.raw);
	setSUBR(netConfig.subnet.raw);
	setSIPR(netConfig.ip.raw);

	// ignore dhcpEnabled as that is set via enableDHCP
	// dhcpEnabled = netConfig.dhcpEnabled
	dnsAddress = netConfig.dns;

	// TODO: dns + dhcpEnabled
}

NetConfig W5500HC::getConfig() {
//	wiz_NetInfo wNetInfo;
//	wizchip_getnetinfo(&wNetInfo);
//
//	NetConfig netConfig = { 0 };
//	MemcopyConfig(netConfig.ipAddress, wNetInfo.ip);
//	MemcopyConfig(netConfig.subnet, wNetInfo.sn);
//	MemcopyConfig(netConfig.gateway, wNetInfo.gw);
//	MemcopyConfig(netConfig.dns, wNetInfo.dns);
//	MemcopyConfig(netConfig.macAddress, wNetInfo.mac);
//
//	netConfig.dhcpEnabled = wNetInfo.dhcp == NETINFO_DHCP;
//
//	return netConfig;

	NetConfig netConfig;

	getSHAR(netConfig.mac.raw);
	getGAR(netConfig.gateway.raw);
	getSUBR(netConfig.subnet.raw);
	getSIPR(netConfig.ip.raw);

	netConfig.dhcpEnabled = dhcpEnabled;
	netConfig.dns = dnsAddress;

	return netConfig;
}

bool W5500HC::phyLinkStatus() {
	auto state = wizphy_getphylink();
	if (state == -1) {
		ETH_Printf("UNABLE TO GET PHY STATE");

		return false;
	}

	return state == PHY_LINK_ON;
}

bool W5500HC::enableDHCP() {
	if (!(initSuccess && phyLinkStatus()))
		return false;

	NetConfig netConfig = getConfig();

	netConfig.mac = {0x00, 0x08, 0xDC, 0x11, 0x22, 0x33 };

	dhcpSuccess = hw.initDHCP(&netConfig);
	if(dhcpSuccess) {
		setConfig(netConfig);

	}

	return dhcpSuccess;
}

bool W5500HC::init(void *config) {
	hwConfig = *(W5500Config*) config;
	initSuccess = hw.init(hwConfig);

	return initSuccess;
}

// returns true if PHY becomes available before timeout
bool W5500HC::waitForPhyLink() {
	uint32_t start = secondsCounter;
	while (!phyLinkStatus()) {
		if (secondsCounter - start > phyTimeoutSeconds) {
			phyTimeout = true;
			return false;
		}
	}

	return true;
}

W5500Socket& W5500HC::getFreeSocket() {

	// NOTE:
	// static is for testing purposes ONLY
	// socket objects will eventually be stored in an array in W5500HC/W5500Interface
	static W5500Socket sock((int)W5500Interface::Socket::DHCP + 1);

	sock.ethHC = this;

	return sock;
}

W5500HC::~W5500HC() {

}

void W5500HC::oneSecondPassed() {
	secondsCounter++;
	DHCP_time_handler();
}

} /* namespace JOELIB */
