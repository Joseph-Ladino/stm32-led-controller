/*
 * StripAnimation.hpp
 *
 *  Created on: Jul 19, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_
#define JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_
#include <string_view>
#include "WS28xxStrip.hpp"

namespace JLED {

struct StripAnimation {
	inline virtual std::string_view getName() {
		return "none";
	}


	inline void run();
};

class StripAnimationManager {
public:
	
	StripAnimationManager();
};

} /* namespace JLED */

#endif /* JLED_INCLUDE_STRIPANIMATIONMANAGER_HPP_ */
