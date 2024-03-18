#include "gtest/gtest.h"
#include "CountdownTimer.hpp"

class TestTimer: public CountdownTimer {
public:
	static void resetTick() {
		tick = 0;
	}
	static void setTick(uint32_t ms) {
		tick = ms;
	}
	static void addTick(uint32_t ms) {
		tick += ms;
	}

	using CountdownTimer::CountdownTimer;
};

TEST(CountdownTimer, ExpiredOnInitialize) {
	TestTimer::setTick(25);
	TestTimer t;
	EXPECT_TRUE(t.expired());

	TestTimer::resetTick();
}

TEST(CountdownTimer, BasicUse) {
	TestTimer t1(1), t2(5), t3(10);

	for (int i = 0; i < 20; i++) {
		TestTimer::msTick();

		if (i >= 1) {
			EXPECT_TRUE(t1.expired());
		}

		if (i >= 5) {
			EXPECT_TRUE(t2.expired());
		}

		if (i >= 10) {
			EXPECT_TRUE(t1.expired());
		}
	}

	TestTimer::resetTick();
}

TEST(CountdownTimer, Overflow) {
	TestTimer::setTick(UINT32_MAX - 10);
	TestTimer t(20);
	TestTimer::addTick(30);

	EXPECT_TRUE(t.expired());

	TestTimer::resetTick();
}

TEST(CountdownTimer, left_ms) {
	TestTimer t(1000);
	EXPECT_FALSE(t.expired());
	EXPECT_EQ(t.left_ms(), 1000);
}
