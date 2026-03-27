/*
 * StripAnimation.hpp
 *
 *  Created on: Jul 19, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_
#define JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_
#include <string_view>
#include <memory>
#include "StripAnimation.hpp"

namespace JLED {

class StripAnimationManager {
public:

	enum AnimationType {
		NONE, RAINBOW, TEST,
	};

	AnimationType activeAnimation;
	std::unique_ptr<StripAnimation> animInst;

	inline std::string_view getName() {
		return animInst->getName();
	}

	inline void run(StripDataInterface *strip) {
		animInst->run(strip);
	}

	inline static std::string_view animTypes[] = { "NONE", "RAINBOW", "TEST" };
	inline static constexpr uint16_t numAnimTypes = sizeof(animTypes) / sizeof(std::string_view);

	inline static std::string_view* getAnimationTypes() {
		return animTypes;
	}

	inline static AnimationType getAnimationTypeFromName(std::string_view name) {
		for (uint16_t i = 0; i < numAnimTypes; i++) {
			if (name == animTypes[i]) return (AnimationType) i;
		}

		return NONE;
	}

	inline void setAnimationType(AnimationType anim) {

#define setInst(type) animInst.reset(new type())

		switch (anim) {
			case RAINBOW:
				setInst(RainbowAnimation);
				break;
			case TEST:
				setInst(TestAnimation);
				break;

			default:
				case NONE:
				setInst(StripAnimation);
				break;
		}
	}

	inline void setAnimationTypeFromName(std::string_view name) {
		setAnimationType(getAnimationTypeFromName(name));
	}

	StripAnimationManager(AnimationType animType = NONE) : activeAnimation(animType) {
		setAnimationType(animType);
	}
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_ */
