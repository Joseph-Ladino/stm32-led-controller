/*
 * EthernetMAC.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_ETHERNETMAC_HPP_
#define JETHERNET_INCLUDE_ETHERNETMAC_HPP_
#include <cstdint>

namespace JETHERNET {

struct EthernetMAC {
	union {
		uint64_t num :48; // 48 bit int, use less memory i think
		uint8_t raw[6];
	};

	// returns max 17 chars (18 w/ null char)
	int cString(char *buf);

	uint8_t& operator[](uint8_t x);
	const uint8_t& operator[](uint8_t x) const;

	EthernetMAC& operator=(const EthernetMAC &rhs) = default;
	EthernetMAC& operator=(const uint32_t mac);
	bool operator==(const EthernetMAC &rhs) const;
	bool operator!=(const EthernetMAC &rhs) const;

	EthernetMAC();
	EthernetMAC(EthernetMAC &addr) = default;
	EthernetMAC(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4,
			uint8_t n5);
	EthernetMAC(uint64_t mac);
};

} /* namespace JETHERNET */

#endif /* JETHERNET_INCLUDE_ETHERNETMAC_HPP_ */

