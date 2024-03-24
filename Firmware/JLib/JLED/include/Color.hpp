/*
 * Color.hpp
 *
 *  Created on: Mar 21, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_COLOR_HPP_
#define JLED_INCLUDE_COLOR_HPP_
#include <cstdint>

namespace JLED {

// this class is largely based off of FastLED
// https://github.com/FastLED/FastLED/blob/master/src/pixeltypes.h
// MIT license: https://github.com/FastLED/FastLED/tree/master?tab=MIT-1-ov-file#readme
struct Color {
	union {
		// order is defined "backwards" to correct ordering in num
		struct {
			uint8_t w; // optional
			uint8_t b;
			uint8_t g;
			uint8_t r;
		};
		uint8_t raw[4];
		uint32_t num = 0;
	};

	// ordering is reverse for raw array
	uint8_t& operator[] (uint8_t n);
	const uint8_t& operator[] (uint8_t n) const;

	bool operator==(const Color &rhs) const;
	bool operator!=(const Color &rhs) const;

	Color& operator=(const uint32_t rhs);
	Color& operator=(const Color& rhs) = default;

	Color() = default;
	Color(Color& c) = default;
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_COLOR_HPP_ */
