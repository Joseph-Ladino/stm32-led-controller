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
	uint8_t mac[6];    ///< Source Mac Address
	uint8_t ip[4];     ///< Source IP Address
	uint8_t sn[4];     ///< Subnet Mask
	uint8_t gw[4];     ///< Gateway IP Address
	uint8_t dns[4];    ///< DNS server IP Address
	bool dhcpEnabled;  ///< true - DHCP, false - Static
};

class EthernetHC {
public:

	virtual void setConfig(NetConfig& netConfig);
	virtual NetConfig getConfig();

	virtual NetConfig getConfigFromDHCP();
	virtual bool applyConfigFromDHCP();
	virtual bool phyAvailable();
	virtual bool init();
};

#endif /* INC_ETHERNETHC_H_ */
