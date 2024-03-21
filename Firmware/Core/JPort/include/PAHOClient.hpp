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
#include <functional>

#ifndef MQTTCLIENT_QOS2
#define MQTTCLIENT_QOS2 2
#endif

#include "MQTTClient.h"

namespace JMQTT {

struct PAHONetwork {
	JETHERNET::EthernetSOCK *sock = nullptr;
	ClientConfig conf;
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

	operator bool() const {
		return sock != nullptr;
	}
};

class PAHOClient {
public:
	using connectCallbackFunc = std::function<void(JMQTT::PAHOClient&)> ;
	using messageCallbackFunc = std::function<void(Message)>;

private:
	PAHONetwork net;
	MQTT::Client<PAHONetwork, CountdownTimer> client;
	messageCallbackFunc onMessageCallback;
	connectCallbackFunc onConnectCallback;

public:

	void pahoMessageReceived(MQTT::MessageData &data);
	bool connect();

	bool publish(Message msg);
	bool subscribe(string_view topic, QOS qos = QOS::QOS0);
	bool connect(JETHERNET::EthernetSOCK &sock, const ClientConfig config);
	void disconnect();
	bool reconnect();

	void setMessageCallback(messageCallbackFunc func);
	void setConnectCallback(connectCallbackFunc func);

	bool update(uint16_t timeout);
	bool isConnected();

	PAHOClient();
	~PAHOClient();
};

} /* namespace JMQTT */

#endif /* JPORT_INCLUDE_PAHOCLIENT_HPP_ */
