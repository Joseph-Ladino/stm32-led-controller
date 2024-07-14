/*
 * StripTypes.hpp
 *
 *  Created on: Jul 13, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_STRIPTYPES_HPP_
#define JLED_INCLUDE_STRIPTYPES_HPP_

#include "Color.hpp"

#define ASSERT_STRIPTYPE_IS_DERIVED(StripType) static_assert(std::is_base_of_v<WSStripType<StripType::bytesPerPixel, StripType::zeroHighNs, StripType::oneHighNs, StripType::pwmPeriodNs>, StripType>, "StripType must be derived from JLED::StripType")

namespace JLED {

template<uint8_t BytesPerPixel, uint16_t ZeroHighNs, uint16_t OneHighNs, uint16_t PwmPeriodNs>
struct WSStripType {
	static constexpr uint8_t bytesPerPixel = BytesPerPixel, bitsPerPixel = BytesPerPixel * 8;
	static constexpr uint16_t zeroHighNs = ZeroHighNs, oneHighNs = OneHighNs, pwmPeriodNs = PwmPeriodNs;

	static constexpr uint32_t getOrderedBytes(uint32_t pixel);
	static constexpr uint32_t orderedToRGBW(uint32_t pixel);
};

template<uint16_t ZeroHighNs, uint16_t OneHighNs, uint16_t PwmPeriodNs>
struct GRBStripType: public WSStripType<3, ZeroHighNs, OneHighNs, PwmPeriodNs> {
	inline constexpr static uint32_t getOrderedBytes(uint32_t pixelRGB) {
		return pixelRGB & 0x00FF00 << 8 | pixelRGB & 0xFF0000 | pixelRGB & 0x0000FF;
	}
	
	inline constexpr static uint32_t orderedToRGBW(uint32_t pixelGRB) {
		return pixelGRB & 0x00FF00 << 8 | pixelGRB & 0xFF0000 >> 8 | pixelGRB & 0x0000FF;
	}
};

using WS2815StripType = GRBStripType<350, 700, 1250>;

}
;

#endif /* JLED_INCLUDE_STRIPTYPES_HPP_ */
