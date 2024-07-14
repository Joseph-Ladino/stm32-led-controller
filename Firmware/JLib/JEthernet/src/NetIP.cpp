/*
 * EthernetIP.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */
#include <NetIP.hpp>
#include <cstdio>

namespace JETHERNET {

int NetIP::cString(char *buf) {
	return sprintf(buf, "%u.%u.%u.%u", raw[0], raw[1], raw[2], raw[3]);
}

uint8_t& NetIP::operator [](uint8_t x) {
	return raw[x];
}

const uint8_t& NetIP::operator [](uint8_t x) const {
	return raw[x];
}

NetIP& NetIP::operator =(const uint32_t ip) {
	num = ip;
	return *this;
}

NetIP::NetIP(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3) {
	raw[0] = n0;
	raw[1] = n1;
	raw[2] = n2;
	raw[3] = n3;
}

NetIP::NetIP(uint32_t ip) : num(ip) {
}

bool NetIP::operator ==(const NetIP &rhs) const {
	return num == rhs.num;
}

bool NetIP::operator !=(const NetIP &rhs) const {
	return num != rhs.num;
}

}

