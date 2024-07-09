#include <gtest/gtest.h>
#include "WS2815Strip.hpp"

using JLED::Color;
using LEDStrip = JLED::WS2815Strip<10>;

TEST(WS2815Strip, InitializeToZero) {
	LEDStrip led { };

	for (int i = 0; i < led.NUM_PIXELS; i++) {
		ASSERT_EQ(led[i], 0);
	}
}

TEST(WS2815Strip, SetGet) {
	LEDStrip led { };
	Color col = 0xFF00FF;
	led.set(4, col);

	EXPECT_EQ(led.at(4), col);
	EXPECT_EQ(led[4], 0xFF00FF);

	led[6] = 0x00FF00;
	EXPECT_EQ(led.at(6), (Color)0x00FF00);
	EXPECT_EQ(led[6], 0x00FF00);
}

TEST(WS2815Strip, SetAll) {
	LEDStrip led { };
	uint32_t colNum = 0xFFFF00;
	led.setAll(0xFFFF00);

	for (int i = 0; i < led.NUM_PIXELS; i++) {
		EXPECT_EQ(led[i], colNum);
	}

	Color colObj = 0x00FFFF;
	led.setAll(colObj);
	for (int i = 0; i < led.NUM_PIXELS; i++) {
		EXPECT_EQ((Color)led[i], colObj);
	}
}

TEST(WS2815Strip, Iterator) {
	LEDStrip led { };
	led.setAll(0xFF0000);
	led[6] = 0x0000FF;

	auto it = led.begin();
	auto end = led.end();

	EXPECT_EQ(*it, 0xFF0000);
	EXPECT_EQ(*end, 0xFF0000);
	EXPECT_NE(it, end);

	int i = 0;
	for(auto c : led) {
		if(i++ == 6) EXPECT_EQ(c, 0x0000FF);
		else EXPECT_EQ(c, 0xFF0000);
	}

	auto np = led.NUM_PIXELS;
	EXPECT_EQ(i, np - 1);
}
