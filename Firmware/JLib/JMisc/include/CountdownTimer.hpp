/*
 * CountdownTimer.hpp
 *
 *  Created on: Mar 11, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_
#define JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_
#include "stm32g0xx_hal.h"

struct CountdownTimer {

	void countdown_ms(uint32_t ms);
	void countdown(uint16_t sec);

	uint32_t left_ms();
	bool expired();

	CountdownTimer();
	CountdownTimer(uint32_t ms);

private:
	uint32_t endMs;
};

#endif /* JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_ */
