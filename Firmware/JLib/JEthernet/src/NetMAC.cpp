/*
 * EthernetMAC.cpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */
#include <NetMAC.hpp>
#include <cstdio>

namespace JETHERNET {
int NetMAC::cString(char *buf) {
	return sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", raw[0], raw[1], raw[2], raw[3], raw[4], raw[5]);
}

uint8_t& NetMAC::operator [](uint8_t x) {
	return raw[x];
}

const uint8_t& NetMAC::operator [](uint8_t x) const {
	return raw[x];
}

NetMAC& NetMAC::operator =(const uint32_t mac) {
	num = mac;
	return *this;
}

NetMAC::NetMAC() : num(0) {
}

NetMAC::NetMAC(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5) {
	raw[0] = n0;
	raw[1] = n1;
	raw[2] = n2;
	raw[3] = n3;
	raw[4] = n4;
	raw[5] = n5;
}

bool NetMAC::operator ==(const NetMAC &rhs) const {
	return num == rhs.num;
}

bool NetMAC::operator !=(const NetMAC &rhs) const {
	return num != rhs.num;
}

NetMAC::NetMAC(uint64_t mac) : num(mac) {
}

}
