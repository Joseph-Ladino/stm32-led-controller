/*
 * EthernetMAC.cpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */
#include <cstdio>
#include "EthernetMAC.hpp"

namespace JETHERNET {
int EthernetMAC::cString(char *buf) {
	return sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", raw[0], raw[1], raw[2],
			raw[3], raw[4], raw[5]);
}

uint8_t& EthernetMAC::operator [](uint8_t x) {
	return raw[x];
}

const uint8_t& EthernetMAC::operator [](uint8_t x) const {
	return raw[x];
}

EthernetMAC& EthernetMAC::operator =(const uint32_t mac) {
	num = mac;
	return *this;
}

EthernetMAC::EthernetMAC() :
		num(0) {
}

EthernetMAC::EthernetMAC(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3,
		uint8_t n4, uint8_t n5) {
	raw[0] = n0;
	raw[1] = n1;
	raw[2] = n2;
	raw[3] = n3;
	raw[4] = n4;
	raw[5] = n5;
}

bool EthernetMAC::operator ==(const EthernetMAC &rhs) const {
	return num == rhs.num;
}

bool EthernetMAC::operator !=(const EthernetMAC &rhs) const {
	return num != rhs.num;
}

EthernetMAC::EthernetMAC(uint64_t mac) :
		num(mac) {
}

}
