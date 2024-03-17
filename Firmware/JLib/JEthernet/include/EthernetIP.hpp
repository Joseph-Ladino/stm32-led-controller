/*
 * EthernetIP.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_ETHERNETIP_HPP_
#define JETHERNET_INCLUDE_ETHERNETIP_HPP_

#include <cstdint>

namespace JETHERNET {

struct EthernetIP {
	union {
		uint32_t num = 0;
		uint8_t raw[4];
	};

	// returns max 15 chars (16 w/ null char)
	int cString(char *buf);

	uint8_t& operator[](uint8_t x);
	const uint8_t& operator[](uint8_t x) const;

	EthernetIP& operator=(const EthernetIP &rhs) = default;
	EthernetIP& operator=(const uint32_t ip);
	bool operator==(const EthernetIP &rhs) const;
	bool operator!=(const EthernetIP &rhs) const;

	EthernetIP() = default;
	EthernetIP(EthernetIP &addr) = default;
	EthernetIP(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3);
	EthernetIP(uint32_t ip);
};

}

#endif /* JETHERNET_INCLUDE_ETHERNETIP_HPP_ */
