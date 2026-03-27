/*
 * ColorCorrection.hpp
 *
 *  Created on: Jul 16, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_COLORCORRECTION_HPP_
#define JLED_INCLUDE_COLORCORRECTION_HPP_

namespace JLED {

#include "GammaCorrection.hpp"

// borrowed from https://github.com/FastLED/FastLED/blob/master/src/color.h
// FASTLed license: https://github.com/FastLED/FastLED/blob/master/LICENSE

enum class ColorCorrection : uint32_t {
	WS2815 = 0xFFB0F0,
	Uncorrected = 0xFFFFFF
};

enum class TemperatureCorrection : uint32_t {
	// Black Body Radiators
	// @{
	/// 1900 Kelvin
	Candle = 0xFF9329 /* 1900 K, 255, 147, 41 */,
	/// 2600 Kelvin
	Tungsten40W = 0xFFC58F /* 2600 K, 255, 197, 143 */,
	/// 2850 Kelvin
	Tungsten100W = 0xFFD6AA /* 2850 K, 255, 214, 170 */,
	/// 3200 Kelvin
	Halogen = 0xFFF1E0 /* 3200 K, 255, 241, 224 */,
	/// 5200 Kelvin
	CarbonArc = 0xFFFAF4 /* 5200 K, 255, 250, 244 */,
	/// 5400 Kelvin
	HighNoonSun = 0xFFFFFB /* 5400 K, 255, 255, 251 */,
	/// 6000 Kelvin
	DirectSunlight = 0xFFFFFF /* 6000 K, 255, 255, 255 */,
	/// 7000 Kelvin
	OvercastSky = 0xC9E2FF /* 7000 K, 201, 226, 255 */,
	/// 20000 Kelvin
	ClearBlueSky = 0x409CFF /* 20000 K, 64, 156, 255 */,
	// @}

	// Gaseous Light Sources
	// @{
	/// Warm (yellower) flourescent light bulbs
	WarmFluorescent = 0xFFF4E5 /* 0 K, 255, 244, 229 */,
	/// Standard flourescent light bulbs
	StandardFluorescent = 0xF4FFFA /* 0 K, 244, 255, 250 */,
	/// Cool white (bluer) flourescent light bulbs
	CoolWhiteFluorescent = 0xD4EBFF /* 0 K, 212, 235, 255 */,
	/// Full spectrum flourescent light bulbs
	FullSpectrumFluorescent = 0xFFF4F2 /* 0 K, 255, 244, 242 */,
	/// Grow light flourescent light bulbs
	GrowLightFluorescent = 0xFFEFF7 /* 0 K, 255, 239, 247 */,
	/// Black light flourescent light bulbs
	BlackLightFluorescent = 0xA700FF /* 0 K, 167, 0, 255 */,
	/// Mercury vapor light bulbs
	MercuryVapor = 0xD8F7FF /* 0 K, 216, 247, 255 */,
	/// Sodium vapor light bulbs
	SodiumVapor = 0xFFD1B2 /* 0 K, 255, 209, 178 */,
	/// Metal-halide light bulbs
	MetalHalide = 0xF2FCFF /* 0 K, 242, 252, 255 */,
	/// High-pressure sodium light bulbs
	HighPressureSodium = 0xFFB74C /* 0 K, 255, 183, 76 */,
	// @}

	/// Uncorrected temperature (0xFFFFFF)
	Uncorrected = 0xFFFFFF /* 255, 255, 255 */
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_COLORCORRECTION_HPP_ */
