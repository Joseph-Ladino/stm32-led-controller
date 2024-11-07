/*
 * EthernetIP.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_NETIP_HPP_
#define JETHERNET_INCLUDE_NETIP_HPP_

#include <cstdint>

namespace JETHERNET {

/**
 * @brief IP address stored internally as a uint32
 * @note MSB is raw[0], LSB is raw[3]
 * @see NetSock
 * @see NetConfig
 * @see EthernetHC
 */
struct NetIP {
	union {
		uint32_t num = 0;
		uint8_t raw[4];
	};

	/**
	 * @brief Print formatted IP address to output buffer
	 * @note maximum 15 characters written (16 w/ null char)
	 * @param buf output buffer
	 * @return number of bytes written
	 */
	int cString(char *buf);

	/**
	 * @brief Access IP bytes
	 * @param index (0-3)
	 * @return byte representing part of an IP address
	 */
	uint8_t& operator[](uint8_t index);

	/**
	 * @brief Access IP bytes
	 * @param index (0-3)
	 * @return byte representing part of an IP address
	 */
	const uint8_t& operator[](uint8_t index) const;

	NetIP& operator=(const NetIP &rhs) = default;
	NetIP& operator=(const uint32_t ip);
	bool operator==(const NetIP &rhs) const;
	bool operator!=(const NetIP &rhs) const;

	NetIP() = default;
	NetIP(NetIP &addr) = default;
	NetIP(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3);
	NetIP(uint32_t ip);
};

}

#endif /* JETHERNET_INCLUDE_NETIP_HPP_ */
