#include "gtest/gtest.h"
#include "NetIP.hpp"

using IP = JETHERNET::NetIP;

TEST(EthernetIP, BasicUse) {
	IP ip1 = { 192, 168, 1, 1 };
	IP ip2 = ip1;

	EXPECT_EQ(ip1, ip2);

	ip2[3] = 2;

	EXPECT_NE(ip1, ip2);
	EXPECT_EQ(ip2[3], 2);
	EXPECT_EQ(((ip2.num >> 24) & 0xff), 2);
}

TEST(EthernetIP, CreateString) {
	char buf[20];
	IP ip;
	int len = 0;

	ip = {255, 255, 255, 255};
	len = ip.cString(buf);
	EXPECT_STREQ(buf, "255.255.255.255");
	EXPECT_EQ(len, 15);

	ip = {255, 128, 64, 32};
	len = ip.cString(buf);
	EXPECT_STREQ(buf, "255.128.64.32");
	EXPECT_EQ(len, 13);
}
