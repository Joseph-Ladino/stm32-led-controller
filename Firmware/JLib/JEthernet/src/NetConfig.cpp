/*
 * NetConfig.c++
 *
 *  Created on: Mar 15, 2024
 *      Author: user
 */

#include "NetConfig.hpp"

namespace JETHERNET {

int NetConfig::cString(char *buf) {
	char temp[20];

	mac.cString(temp);
	int len = sprintf(buf, "MAC Address : %s\n", temp);

	ip.cString(temp);
	len += sprintf(buf + len, "IP  Address : %s\n", temp);

	subnet.cString(temp);
	len += sprintf(buf + len, "Subnet Mask : %s\n", temp);

	gateway.cString(temp);
	len += sprintf(buf + len, "Gateway     : %s\n", temp);

	dns.cString(temp);
	len += sprintf(buf + len, "DNS Server  : %s\n", temp);

	return len;
}

}
