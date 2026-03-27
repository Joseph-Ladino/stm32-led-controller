/*
 * StripAnimation.hpp
 *
 *  Created on: Jul 30, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_STRIPANIMATION_HPP_
#define JLED_INCLUDE_STRIPANIMATION_HPP_

#include <StripDataInterface.hpp>

namespace JLED {

struct StripAnimation {
	inline virtual std::string_view getName() {
		return "none";
	}

	inline virtual void run(StripDataInterface* strip) {
	}
};

struct RainbowAnimation final : public StripAnimation {
	inline std::string_view getName() final {
		return "rainbow";
	}

	inline void run(StripDataInterface* strip) final {
		strip->setAll(0xFF00FF);
	}
};

struct TestAnimation final : public StripAnimation {
	inline std::string_view getName() final {
		return "test";
	}

	inline void run(StripDataInterface* strip) final {
		strip->setAll(0xFFFF00);
	}
};

}

#endif /* JLED_INCLUDE_STRIPANIMATION_HPP_ */
