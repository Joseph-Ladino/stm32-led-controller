/*
 * W5500Client.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: user
 */

#ifndef JPORT_INCLUDE_PAHOCLIENT_HPP_
#define JPORT_INCLUDE_PAHOCLIENT_HPP_
#include "Client.hpp"
#include "CountdownTimer.hpp"
#include "MQTTClient.h"
#include <functional>

#ifndef MQTTCLIENT_QOS2
#define MQTTCLIENT_QOS2 2
#endif

namespace JMQTT {

struct PAHONetwork {
	JETHERNET::EthernetSOCK *sock = nullptr;
	inline int read(uint8_t *buffer, uint16_t len, uint16_t timeoutMs) {
		if (!sock)
			return -1;

		return sock->read(buffer, len);
	}
	inline int write(uint8_t *buffer, uint16_t len, uint16_t timeoutMs) {
		if (!sock)
			return -1;

		return sock->write(buffer, len);
	}
};

class PAHOClient {
public:
	MQTT::Client<PAHONetwork, CountdownTimer> client;
	PAHONetwork net;
	std::function<void(Message)> handler;

	void publish(Message msg);
	void subscribe(string_view topic, QOS qos = QOS::QOS0);
	bool connect(JETHERNET::EthernetSOCK &sock, ClientConfig config);
	void disconnect();

	void setMessageHandler(const std::function<void(Message)> &func);
	void messageReceived(Message *msg);

	PAHOClient();
	~PAHOClient();

private:
	void pahoMessageReceived(MQTT::MessageData &data);
};

} /* namespace JMQTT */

#endif /* JPORT_INCLUDE_PAHOCLIENT_HPP_ */
