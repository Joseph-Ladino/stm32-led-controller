/*
 * StripBuilder.hpp
 *
 *  Created on: Jul 16, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_STRIPBUILDER_HPP_
#define JLED_INCLUDE_STRIPBUILDER_HPP_

#include "WS28xxStrip.hpp"

namespace JLED {

template<typename StripBase>
class StripBuilder {
	StripBase *strip;
public:
	using LEDBase = StripBase;
	using StripType = typename LEDBase::StripType;
	ASSERT_STRIPBASE_IS_DERIVED(StripBase, StripType)
	;

	inline StripBuilder& setAll(uint32_t color) {
		strip->setAll(color);
		return *this;
	}

	inline StripBuilder& setStaticColor(uint32_t color) {
		strip->setStaticColor(color);
		return *this;
	}

	inline StripBuilder& setPower(bool powerOn) {
		strip->setPower(powerOn);
		return *this;
	}

	inline StripBuilder& setPhysicalPowerCB(typename LEDBase::PhysicalPowerCB cb) {
		strip->setPhysicalPowerCB(cb);
		return *this;
	}

	inline StripBuilder& setBrightness(uint8_t brightness) {
		strip->setBrightness(brightness);
		return *this;
	}

	inline StripBuilder& setEffect() {
		strip->setEffect();
		return *this;
	}

	inline StripBuilder& setMode() {
		strip->setMode();
		return *this;
	}

	inline StripBuilder& setColorCorrection(ColorCorrection cc) {
		strip->setColorCorrection(cc);
		return *this;
	}

	inline StripBuilder& setTemperatureCorrection(TemperatureCorrection tc) {
		strip->setTemperatureCorrection(tc);
		return *this;
	}

	inline StripBuilder& setGammaCorrection(bool on) {
		strip->setGammaCorrection(on);
		return *this;
	}

	inline StripBase* build() {
		return strip;
	}

	inline StripBuilder(StripBase *strip) : strip(strip) {
	}
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_STRIPBUILDER_HPP_ */
