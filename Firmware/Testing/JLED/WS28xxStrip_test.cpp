#include <gtest/gtest.h>
#include "WS28xxStrip.hpp"

#define NEW_LEDSTRIP(numLeds) \
	uint32_t rawColor[numLeds], fxColor[numLeds]; \
	LEDStrip led { numLeds, rawColor, fxColor };


using JLED::Color;
using LEDStrip = JLED::WS28xxStrip<JLED::WS2815StripType>;

TEST(WS28xxStrip, InitializeToZero) {
	NEW_LEDSTRIP(10);

	for (int i = 0; i < led.getNumPixels(); i++) {
		ASSERT_EQ(led[i], 0);
	}
}

TEST(WS28xxStrip, SetGet) {
	NEW_LEDSTRIP(10);

	Color col = 0xFF00FF;
	led[4] = col;

	EXPECT_EQ((Color)led[4], col);
	EXPECT_EQ(led[4], 0xFF00FF);

	led[6] = 0x00FF00;
	EXPECT_EQ((Color)led[6], (Color)0x00FF00);
	EXPECT_EQ(led[6], 0x00FF00);
}

TEST(WS28xxStrip, SetAll) {
	NEW_LEDSTRIP(10);
	uint32_t colNum = 0xFFFF00;
	led.setAll(0xFFFF00);

	for (int i = 0; i < led.getNumPixels(); i++) {
		EXPECT_EQ(led[i], colNum);
	}

	Color colObj = 0x00FFFF;
	led.setAll(colObj);
	for (int i = 0; i < led.getNumPixels(); i++) {
		EXPECT_EQ((Color)led[i], colObj);
	}
}

TEST(WS28xxStrip, Iterator) {
	NEW_LEDSTRIP(10);
	led.setAll(0xFF0000);
	led[6] = 0x0000FF;

	auto it = led.begin();
	auto end = led.end();

	EXPECT_EQ(*it, 0xFF0000);
	EXPECT_EQ(*(--end), 0xFF0000);
	EXPECT_NE(it, end);

	int i = 0;
	for(auto c : led) {
		if(i++ == 6) EXPECT_EQ(c, 0x0000FF);
		else EXPECT_EQ(c, 0xFF0000);
	}

	auto np = led.getNumPixels();
	EXPECT_EQ(i, np);
}
