/*
 * MQTTInterface.hpp
 *
 *  Created on: Mar 6, 2024
 *      Author: DRTNT
 */

#ifndef JETHERNET_INCLUDE_MQTTINTERFACE_HPP_
#define JETHERNET_INCLUDE_MQTTINTERFACE_HPP_

#include <cstdio>
#include "MQTTClient.h"
#include "CountdownTimer.hpp"

namespace JMQTT {

struct W5500MqttNetwork {
	int socketNum;
//		int (*mqttread) (Network*, unsigned char*, int, long);
//		int (*mqttwrite) (Network*, unsigned char*, int, long);
//		void (*disconnect) (Network*);
	
	bool isConnected();
	void setSocket(int socketNum);

	int connect(const char *hostname, uint16_t port);
	int connect(uint8_t *ip, uint16_t port);
	void disconnect();

	int read(unsigned char *buffer, int len, int timeout_ms);
	int write(unsigned char *buffer, int len, int timeout);

	W5500MqttNetwork();
	W5500MqttNetwork(int socketNum);
};

using W5500MqttClient = MQTT::Client<W5500MqttNetwork, CountdownTimer, 500>;
}

#endif /* JETHERNET_INCLUDE_MQTTINTERFACE_HPP_ */
