/*
 * Color.hpp
 *
 *  Created on: Mar 21, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_COLOR_HPP_
#define JLED_INCLUDE_COLOR_HPP_

#include <cstdint>
#include "ColorCorrection.hpp"

namespace JLED {

/**
 * @brief RGBW color stored internally as uint32.
 * this class is largely based off of <a href="https://github.com/FastLED/FastLED/blob/master/src/pixeltypes.h">FastLED</a>
 * (<a href="https://github.com/FastLED/FastLED/tree/master?tab=MIT-1-ov-file#readme">MIT license</a>)
 */
struct Color {
	union {
		/**
		 * @note order of fields is defined "backwards" to correct the ordering when accessed as uint32 (num)
		 */
		struct {
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t w; ///< optional
		};
		uint8_t raw[4];
		uint32_t num = 0;
	};

	// ordering is reverse for raw array
	inline uint8_t& operator[](uint8_t n) {
		return (n > 3) ? raw[0] : raw[3 - n];
	}
	inline const uint8_t& operator[](uint8_t n) const {
		return (n > 3) ? raw[0] : raw[3 - n];
	}

	inline operator uint32_t() const {
		return num;
	}

	inline bool operator==(const Color &rhs) const {
		return num == rhs.num;
	}
	inline bool operator!=(const Color &rhs) const {
		return num != rhs.num;
	}

	inline bool operator==(const uint32_t rhs) const {
		return num == rhs;
	}
	inline bool operator!=(const uint32_t rhs) const {
		return num != rhs;
	}

	Color& operator=(const Color &rhs) = default;
	Color& operator=(const uint32_t rhs) {
		num = rhs;
		return *this;
	}

	Color() = default;
	Color(Color &c) = default;

	inline constexpr Color(uint32_t num) : num(num) {
	}
	inline constexpr Color(ColorCorrection col) : num((uint32_t) col) {
	}
	inline constexpr Color(TemperatureCorrection col) : num((uint32_t) col) {
	}
	inline constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) : b(b), g(g), r(r), w(w) {
	}
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_COLOR_HPP_ */
