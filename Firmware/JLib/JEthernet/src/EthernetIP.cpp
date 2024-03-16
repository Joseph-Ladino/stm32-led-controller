/*
 * EthernetIP.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */
#include "EthernetIP.hpp"
#include "stdio.h"

namespace JETHERNET {

int EthernetIP::cString(char *buf) {
	return sprintf(buf, "%u.%u.%u.%u", raw[0], raw[1], raw[2], raw[3]);
}

uint8_t& EthernetIP::operator [](uint8_t x) {
	return raw[x];
}

const uint8_t& EthernetIP::operator [](uint8_t x) const {
	return raw[x];
}

EthernetIP& EthernetIP::operator =(const uint32_t ip) {
	num = ip;
	return *this;
}

EthernetIP::EthernetIP(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3) {
	raw[0] = n0;
	raw[1] = n1;
	raw[2] = n2;
	raw[3] = n3;
}

EthernetIP::EthernetIP(uint32_t ip) :
		num(ip) {
}

}

