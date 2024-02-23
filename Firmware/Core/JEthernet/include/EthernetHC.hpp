/*
 * EthernetHC.h
 *
 *  Created on: Feb 21, 2024
 *      Author: user
 */

#ifndef INC_ETHERNETHC_H_
#define INC_ETHERNETHC_H_

#include "stm32g0xx_hal.h"

struct NetConfig {
	uint8_t macAddress[6];    ///< Source Mac Address
	uint8_t ipAddress[4];     ///< Source IP Address
	uint8_t subnet[4];     ///< Subnet Mask
	uint8_t gateway[4];     ///< Gateway IP Address
	uint8_t dns[4];    ///< DNS server IP Address
	bool dhcpEnabled;  ///< true - DHCP, false - Static
};

class EthernetHC {
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

	~EthernetHC() = default;
};

#endif /* INC_ETHERNETHC_H_ */
