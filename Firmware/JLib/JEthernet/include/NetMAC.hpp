/*
 * EthernetMAC.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_NETMAC_HPP_
#define JETHERNET_INCLUDE_NETMAC_HPP_
#include <cstdint>

namespace JETHERNET {

/**
 * @brief MAC address stored internally as a 48 bit bitset
 * @note MSB is raw[0], LSB is raw[5]
 * @see NetSock
 * @see NetConfig
 * @see EthernetHC
 */
struct NetMAC {
	union {
		uint64_t num : 48; // 48 bit int, use less memory i think
		uint8_t raw[6];
	};

	/**
	 * @brief Print formatted MAC address to output buffer
	 * @note maximum 17 characters written (18 w/ null char)
	 * @param buf output buffer
	 * @return number of bytes written
	 */
	int cString(char *buf);

	/**
	 * @brief Access MAC bytes
	 * @param index (0-5)
	 * @return byte representing part of a MAC address
	 */
	uint8_t& operator[](uint8_t index);

	/**
	 * @brief Access MAC bytes
	 * @param index (0-5)
	 * @return byte representing part of a MAC address
	 */
	const uint8_t& operator[](uint8_t index) const;

	NetMAC& operator=(const NetMAC &rhs) = default;
	NetMAC& operator=(const uint32_t mac);
	bool operator==(const NetMAC &rhs) const;
	bool operator!=(const NetMAC &rhs) const;

	NetMAC();
	NetMAC(NetMAC &addr) = default;
	NetMAC(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4,
			uint8_t n5);
	NetMAC(uint64_t mac);
};

} /* namespace JETHERNET */

#endif /* JETHERNET_INCLUDE_NETMAC_HPP_ */

