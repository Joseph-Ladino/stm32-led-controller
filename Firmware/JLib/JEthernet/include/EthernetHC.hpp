/*
 * EthernetHC.h
 *
 *  Created on: Feb 21, 2024
 *      Author: user
 */

#ifndef INC_ETHERNETHC_H_
#define INC_ETHERNETHC_H_

#include <cstdint>
#include "NetConfig.hpp"

namespace JETHERNET {

class NetIP;
class NetSock;
class EthernetHC { // @suppress("Class has a virtual method and non-virtual destructor")
public:
	virtual void setConfig(NetConfig& netConfig) = 0;
	virtual NetConfig getConfig() = 0;

	virtual void msTick() = 0;
	virtual bool enableDHCP(uint16_t timeoutMs) = 0;
	virtual bool phyLinkStatus() = 0;
	virtual bool waitForLink(uint16_t timeoutMs) = 0;
	virtual bool init(void*) = 0;

	// new 03/14
//	virtual void deinit() = 0;
	virtual bool enableDNS(NetIP dns) = 0;
	virtual NetIP domainToIP(const char * domain, uint16_t timeoutMs) = 0;
	virtual NetSock& getFreeSocket() = 0;

	virtual ~EthernetHC() {};
};

}

#endif /* INC_ETHERNETHC_H_ */
