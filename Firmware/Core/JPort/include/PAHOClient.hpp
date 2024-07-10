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
	JETHERNET::NetSock *sock = nullptr;
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

	std::vector<MQTTMessageReceiver*> genericReceivers;
	std::vector<std::pair<std::string, MQTTMessageReceiver*>> specificReceivers;
	uint16_t receiverWriteIndex = 0;

	bool connect();
public:


	bool publish(Message msg) final;
	bool update(uint16_t timeout) final;
	bool subscribe(string_view topic, QOS qos = QOS::QOS0) final;
	bool subscribe(string_view topic, MQTTMessageReceiver* recv, QOS qos = QOS::QOS0) final;

	bool connect(JETHERNET::NetSock &sock, const ClientConfig config) final;
	bool reconnect() final;
	bool reconnect(JETHERNET::NetSock &sock);
	void disconnect() final;
	bool isConnected() final;

	void registerMessageReceiver(MQTTMessageReceiver* recv) final;

	void setMessageCallback(MessageCB func) final;
	void setConnectCallback(ConnectCB func) final;
	void pahoMessageReceived(MQTT::MessageData &data);


	PAHOClient(const uint16_t numReceivers = 10);
	~PAHOClient();
};

} /* namespace JMQTT */

#endif /* JPORT_INCLUDE_PAHOCLIENT_HPP_ */
