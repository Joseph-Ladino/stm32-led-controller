#include <gtest/gtest.h>
#include "Color.hpp"

using Col = JLED::Color;

TEST(Color, ColorOrder) {
	Col c;
	c.w = 0xFF;
	EXPECT_EQ(c.num, 0xFF000000);
	EXPECT_EQ(c[0], 0XFF);

	c.num = 0;
	c.r = 0xFF;
	EXPECT_EQ(c.num, 0x00FF0000);
	EXPECT_EQ(c[1], 0XFF);

	c.num = 0;
	c.g = 0xFF;
	EXPECT_EQ(c.num, 0x0000FF00);
	EXPECT_EQ(c[2], 0XFF);

	c.num = 0;
	c.b = 0xFF;
	EXPECT_EQ(c.num, 0x000000FF);
	EXPECT_EQ(c[3], 0XFF);
}

TEST(Color, Overflow) {
	Col c;
	c.b = 0xF8;
	EXPECT_EQ(c[5], c.b);
}
