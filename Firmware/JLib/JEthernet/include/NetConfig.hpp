/*
 * NetConfig.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_NETCONFIG_HPP_
#define JETHERNET_INCLUDE_NETCONFIG_HPP_

#include "EthernetMAC.hpp"
#include "EthernetIP.hpp"
#include "stdio.h"

namespace JETHERNET {

struct NetConfig {
	EthernetMAC mac;
	EthernetIP ip;
	EthernetIP subnet;
	EthernetIP gateway;
	EthernetIP dns;
	bool dhcpEnabled;

	// returns max 152 chars (153 w/ null char)
	int cString(char* buf);
};

}


#endif /* JETHERNET_INCLUDE_NETCONFIG_HPP_ */
