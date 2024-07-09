/*
 * CountdownTimer.hpp
 *
 *  Created on: Mar 11, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_
#define JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_

#include <cstdint>

/**
 * @brief Generic user-controlled timer with millisecond precision
 */
struct CountdownTimer {

	/**
	 * @brief set timer duration in milliseconds
	 * @param ms milliseconds
	 * @note camelcase naming pattern is not followed due to this class being directly used in <a href="https://modelbasedtesting.co.uk/2014/08/25/porting-a-paho-embedded-c-client/">PAHOMqtt</a>
	 */
	void countdown_ms(uint32_t ms);

	/**
	 * @brief set timer duration in milliseconds
	 * @param ms milliseconds
	 * @note alias for countdown_ms()
	 */
	inline void countdownMS(uint32_t ms) {
		countdown_ms(ms);
	}

	/**
	 * @brief set timer duration in seconds
	 * @param sec seconds
	 */
	void countdown(uint16_t sec);

	/**
	 * @brief remaining timer duration
	 * @return milliseconds left until timer expires
	 * @note camelcase naming pattern is not followed due to this class being directly used in <a href="https://modelbasedtesting.co.uk/2014/08/25/porting-a-paho-embedded-c-client/">PAHOMqtt</a>
	 */
	uint32_t left_ms();

	/**
	 * @brief remaining timer duration
	 * @return milliseconds left until timer expires
	 * @note alias for left_ms()
	 */
	inline uint32_t leftMs() {
		return left_ms();
	}

	/**
	 * @brief check if timer is expired
	 * @return true if timer is expired
	 */
	bool expired();

	CountdownTimer();

	/**
	 * @brief initialize timer with millisecond duration
	 * @param ms milliseconds
	 */
	CountdownTimer(uint32_t ms);

	/**
	 * @brief advance all timer instances by one millisecond
	 * <br/> must be called reliably every millisecond
	 * @warning call this method only once in the program, not once per instance
	 */
	static void msTick();

private:
	uint32_t endMs = 0;

protected:
	inline static uint32_t tick = 0;
};

#endif /* JETHERNET_INCLUDE_COUNTDOWNTIMER_HPP_ */
