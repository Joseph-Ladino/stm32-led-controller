/*
 * Color.cpp
 *
 *  Created on: Mar 21, 2024
 *      Author: user
 */

#include "Color.hpp"

namespace JLED {

uint8_t& Color::operator [](uint8_t n) {
	return (n > 3) ? raw[0] : raw[3 - n];
}

const uint8_t& Color::operator [](uint8_t n) const {
	return (n > 3) ? raw[0] : raw[3 - n];
}

bool Color::operator ==(const Color &rhs) const {
	return num == rhs.num;
}

Color::operator uint32_t() {
	return num;
}

bool Color::operator !=(const Color &rhs) const {
	return num != rhs.num;
}

Color& Color::operator =(const uint32_t rhs) {
	num = rhs;
	return *this;
}

Color::Color(uint32_t num) : num(num) {}



} /* namespace JLED */
