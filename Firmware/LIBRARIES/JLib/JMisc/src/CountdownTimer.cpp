/*
 * CountdwonTimer.cpp
 *
 *  Created on: Mar 11, 2024
 *      Author: user
 */

#include "CountdownTimer.hpp"

void CountdownTimer::countdown_ms(uint32_t ms) {
	lastTimerDuration = ms;
	endMs = tick + ms;
}

void CountdownTimer::countdown(uint16_t sec) {
	countdown_ms(sec * 1000);
}

uint32_t CountdownTimer::left_ms() {
	if (expired()) return 0;
	
	return endMs - tick;
}

bool CountdownTimer::expired() {
	// handles overflow
	// https://luckyresistor.me/2019/07/10/real-time-counter-and-integer-overflow/
	return int32_t(endMs - tick) < 1;
}

void CountdownTimer::msTick() {
	tick++;
}

CountdownTimer::CountdownTimer() {
	countdown_ms(0);
}

CountdownTimer::CountdownTimer(uint32_t ms) {
	countdown_ms(ms);
}
