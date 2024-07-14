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
	setSHAR(netConfig.mac.raw);
	setGAR(netConfig.gateway.raw);
	setSUBR(netConfig.subnet.raw);
	setSIPR(netConfig.ip.raw);
	
	// ignore dhcpEnabled as that is set via enableDHCP
	// dhcpEnabled = netConfig.dhcpEnabled
	if (netConfig.dns.num != 0) dnsAddress = netConfig.dns;
}

NetConfig W5500HC::getConfig() {
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
	if (!(initSuccess && phyLinkStatus())) return false;
	
	NetConfig netConfig = getConfig();
	
	netConfig.mac = { 0x00, 0x08, 0xDC, 0x11, 0x22, 0x33 };
	
	dhcpSuccess = hw.initDHCP(getFreeSocket(), &netConfig, timeoutMs);
	if (dhcpSuccess) {
		dhcpEnabled = true;
		setConfig(netConfig);
	} else {
		dhcpEnabled = false;
	}
	
	return dhcpSuccess;
}

bool W5500HC::enableDNS(NetIP dns) {
	if (dns.num != 0) {
		dnsAddress = dns;
	} else if (dnsAddress.num == 0) {
		return false;
	}
	
	return true;
}

// ip will be 0 if failure
NetIP W5500HC::domainToIP(const char *domain, uint16_t timeoutMs) {
	CountdownTimer t(timeoutMs);
	NetIP ip;
	
	// socket is closed in DNS_run
	hw.initDNS(getFreeSocket());
	while (DNS_run(dnsAddress.raw, (uint8_t*) domain, ip.raw) != 1 && !t.expired())
		;
	
	return ip;
}

void W5500HC::softReset() {
	dhcpEnabled = false;
	dhcpSuccess = false;
	phyTimeout = false;
	dnsAddress = 0;
	
	for (auto &s : sockets) {
		s.close();
		s.disconnect();
	}
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
	static W5500Socket empty;
	
	for (uint8_t i = 0; i < 8; i++) {
		auto &sock = sockets[i];
		if (!sock.isConnected()) return sock;
	}
	
	return empty;
}

void W5500HC::msTick() {
	static uint32_t tick = 0;
	tick++;
	
	if (tick >= 1000) {
		tick = 0;
		DHCP_time_handler();
		DNS_time_handler();
	}
}

W5500HC::~W5500HC() {
	
}

} /* namespace JOELIB */
