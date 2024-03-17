/*
 * W500HC.cpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#include <cstring>
#include <cstdint>

#include "wizchip_conf.h"
#include "dhcp.h"

#include "W5500HC.hpp"
#include "W5500Socket.hpp"
#include "CountdownTimer.hpp"

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

bool W5500HC::enableDHCP(uint16_t timeoutMs) {
	if (!(initSuccess && phyLinkStatus()))
		return false;

	NetConfig netConfig = getConfig();

	netConfig.mac = { 0x00, 0x08, 0xDC, 0x11, 0x22, 0x33 };

	dhcpSuccess = hw.initDHCP(getFreeSocket(), &netConfig, timeoutMs);
	if (dhcpSuccess) {
		setConfig(netConfig);

	}

	return dhcpSuccess;
}

bool W5500HC::init(void *config) {
	hwConfig = *(W5500Config*) config;
	initSuccess = hw.init(hwConfig);

	if (initSuccess) {
		for (uint8_t i = 0; i < 8; i++) {
			sockets[i].socketNum = i;
			sockets[i].ethHC = this;
		}
	}

	return initSuccess;
}

// returns true if PHY becomes available before timeout
bool W5500HC::waitForLink(uint16_t timeoutMs) {
	CountdownTimer timer(timeoutMs);
	while (!(phyLinkStatus() || timer.expired()))
		;

	return phyLinkStatus();
}

W5500Socket& W5500HC::getFreeSocket() {

	static W5500Socket blank;

	for (uint8_t i = 0; i < 8; i++) {
		auto &sock = sockets[i];
		if (!sock.isConnected())
			return sock;
	}

	return blank;

	// NOTE:
	// static is for testing purposes ONLY
	// socket objects will eventually be stored in an array in W5500HC/W5500Interface
//	static W5500Socket sock((int) W5500Interface::Socket::DNS);
//
//	sock.ethHC = this;
//
//	return sock;
}

void W5500HC::msTick() {
	static uint32_t tick = 0;
	tick++;

	if (tick % 1000 == 0)
		DHCP_time_handler();
}

W5500HC::~W5500HC() {

}

} /* namespace JOELIB */
