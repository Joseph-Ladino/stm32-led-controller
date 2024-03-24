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
		if (!this->operator bool())
			return -1;

		return sock->read(buffer, len);
	}
	inline int write(uint8_t *buffer, uint16_t len, uint16_t timeoutMs) {
		if (!this->operator bool())
			return -1;

		return sock->write(buffer, len);
	}

	inline operator bool() const {
		return sock != nullptr && sock->isConnected();
	}
};

class PAHOClient : public JMQTT::Client {
public:
//	using ConnectCB = std::function<void(JMQTT::PAHOClient&)> ;
//	using MessageCB = std::function<void(JMQTT::PAHOClient&, Message)>;

private:
	PAHONetwork net;
	MQTT::Client<PAHONetwork, CountdownTimer> client;
	MessageCB onMessage;
	ConnectCB onConnect;
	bool connect();

public:


	bool publish(Message msg);
	bool subscribe(string_view topic, QOS qos = QOS::QOS0);
	bool update(uint16_t timeout);

	bool connect(JETHERNET::EthernetSOCK &sock, const ClientConfig config);
	bool reconnect();
	bool reconnect(JETHERNET::EthernetSOCK &sock);
	void disconnect();
	bool isConnected();

	void setMessageCallback(MessageCB func);
	void setConnectCallback(ConnectCB func);
	void pahoMessageReceived(MQTT::MessageData &data);


	PAHOClient();
	~PAHOClient();
};

} /* namespace JMQTT */

#endif /* JPORT_INCLUDE_PAHOCLIENT_HPP_ */
