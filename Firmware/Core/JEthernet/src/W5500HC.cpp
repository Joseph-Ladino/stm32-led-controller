/*
 * W500HC.cpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#include <W5500HC.hpp>
#include <stdexcept>
#include <cstring>
#include "wizchip_conf.h"
#include "dhcp.h"

namespace JETHERNET {

#define MemcopyConfig(member1, member2) memcpy(member1, member2, (sizeof(member1) < sizeof(member2)) ? sizeof(member1) : sizeof(member2))

void W5500HC::setConfig(NetConfig &netConfig) {
	wiz_NetInfo wNetInfo = { 0 };
	MemcopyConfig(wNetInfo.ip, netConfig.ipAddress);
	MemcopyConfig(wNetInfo.sn, netConfig.subnet);
	MemcopyConfig(wNetInfo.gw, netConfig.gateway);
	MemcopyConfig(wNetInfo.dns, netConfig.dns);
	MemcopyConfig(wNetInfo.mac, netConfig.macAddress);

	wizchip_setnetinfo(&wNetInfo);
}

NetConfig W5500HC::getConfig() {
	wiz_NetInfo wNetInfo;
	wizchip_getnetinfo(&wNetInfo);

	NetConfig netConfig = { 0 };
	MemcopyConfig(netConfig.ipAddress, wNetInfo.ip);
	MemcopyConfig(netConfig.subnet, wNetInfo.sn);
	MemcopyConfig(netConfig.gateway, wNetInfo.gw);
	MemcopyConfig(netConfig.dns, wNetInfo.dns);
	MemcopyConfig(netConfig.macAddress, wNetInfo.mac);

	netConfig.dhcpEnabled = wNetInfo.dhcp == NETINFO_DHCP;

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
	dhcpSuccess = hw.initDHCP(&netConfig);

	if(dhcpSuccess) setConfig(netConfig);

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

W5500HC::~W5500HC() {
}

void W5500HC::oneSecondPassed() {
	secondsCounter++;
	DHCP_time_handler();
}

} /* namespace JOELIB */
