/*
 * NetConfig.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_NETCONFIG_HPP_
#define JETHERNET_INCLUDE_NETCONFIG_HPP_

#include <NetIP.hpp>
#include <NetMAC.hpp>

namespace JETHERNET {

/**
 * @brief Network configuration
 * @see NetIP
 * @see NetMAC
 * @see NetSock
 * @see EthernetHC
 */
struct NetConfig {
	NetMAC mac;
	NetIP ip;
	NetIP subnet;
	NetIP gateway;
	NetIP dns;
	bool dhcpEnabled;

	/**
	 * @brief Print formatted network config to output buffer
	 * @note maximum 152 characters written (153 w/ null char)
	 * @param buf output buffer
	 * @return number of bytes written
	 */
	int cString(char *buf);
};

}

#endif /* JETHERNET_INCLUDE_NETCONFIG_HPP_ */
