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

Color::Color(uint32_t num) : num(num) {
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) : b(b), g(g), r(r), w(w) {
}

} /* namespace JLED */
