/*
 * EthernetHC.h
 *
 *  Created on: Feb 21, 2024
 *      Author: user
 */

#ifndef INC_ETHERNETHC_H_
#define INC_ETHERNETHC_H_

#include <stdint.h>
#include "NetConfig.hpp"

namespace JETHERNET {

class EthernetSOCK;
class EthernetHC { // @suppress("Class has a virtual method and non-virtual destructor")
protected:
	bool dhcpSuccess = false;
	bool initSuccess = false;
	bool phyTimeout = false;
	uint32_t phyTimeoutSeconds = 10;

	volatile uint32_t secondsCounter = 0;

public:
	virtual void oneSecondPassed() = 0;
	virtual void setConfig(NetConfig& netConfig) = 0;
	virtual NetConfig getConfig() = 0;

	virtual bool enableDHCP() = 0;
	virtual bool phyLinkStatus() = 0;
	virtual bool waitForPhyLink() = 0;
	virtual bool init(void*) = 0;

	// new 03/14
//	virtual void deinit() = 0;
//	virtual bool enableDNS() = 0;
//	virtual bool domainToIP(const char * domain, uint8_t* ipOut) = 0;
	virtual EthernetSOCK& getFreeSocket() = 0;
//	virtual void openSocket(EthernetSOCK& sock) = 0;
//	virtual void closeSocket(EthernetSOCK& sock) = 0;

	virtual ~EthernetHC() {};
};

}

#endif /* INC_ETHERNETHC_H_ */
