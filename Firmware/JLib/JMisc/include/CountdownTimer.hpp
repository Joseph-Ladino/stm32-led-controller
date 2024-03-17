/*
 * CountdownTimer.hpp
 *
 *  Created on: Mar 11, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_
#define JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_

#include <cstdint>

struct CountdownTimer {

	void countdown_ms(uint32_t ms);
	void countdown(uint16_t sec);

	uint32_t left_ms();
	bool expired();

	CountdownTimer();
	CountdownTimer(uint32_t ms);

	static void msTick();

private:
	uint32_t endMs = 0;

protected:
	inline static uint32_t tick = 0;
};

#endif /* JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_ */
