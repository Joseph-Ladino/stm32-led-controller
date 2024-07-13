/*
 * MQTTClient.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: user
 */

#ifndef JMQTT_INCLUDE_CLIENT_HPP_
#define JMQTT_INCLUDE_CLIENT_HPP_

#include <cstdint>
#include <string>
#include <functional>
#include "NetSock.hpp"
#include "SignalListener.hpp"

using std::string_view;
using std::string;

namespace JMQTT {

enum class QOS : uint8_t {
	QOS0 = 0, QOS1, QOS2
};

struct Message {
	string topic = "";
	string payload = "";
	QOS qos = QOS::QOS0;

	inline operator string() const {
		return topic + ": " + payload;
	}
};

enum class MQTTVersion : uint8_t {
	MQTT3 = 3, MQTT311 = 4, MQTT5 = 5,
};

struct ClientConfig {
	MQTTVersion version = MQTTVersion::MQTT311;
	JETHERNET::NetIP brokerIP = 0;
	uint16_t brokerPort = 0;
	string clientName = "JMQTTClient";
	string username = "";
	string password = "";
};

class Client;
struct OnMessageData {
	Client &client;
	Message &message;
};

struct MQTTMessageReceiver: SignalListener<void, OnMessageData> {
	inline void signal(OnMessageData data) final {
		onMessage(data.client, data.message);
	}
	virtual void onMessage(Client&, const Message&) = 0;
};

class Client {
public:
	using ConnectCB = std::function<void(Client&)>;
	using MessageCB = std::function<void(Client&, Message)>;

	virtual bool publish(Message msg) = 0;
	virtual inline bool publish(string topic, string message, QOS qos = QOS::QOS0) final { return publish({topic, message, qos}); }
	virtual bool update(uint16_t timeoutMs) = 0;

	virtual bool subscribe(string_view topic, QOS qos = QOS::QOS0) = 0;
	virtual bool subscribe(string_view topic, MQTTMessageReceiver *recv,
			QOS qos = QOS::QOS0) = 0;

	// stolen from: https://github.com/eclipse/paho.mqtt.embedded-c/blob/master/MQTTClient/src/MQTTClient.h#L497
	virtual inline bool isTopicMatched(string_view topic, string_view filter) final {
		char *curf = (char*) filter.data();
		char *curn = (char*) topic.data();
		char *curn_end = curn + topic.size();

		while (*curf && curn < curn_end) {
			if (*curn == '/' && *curf != '/')
				break;
			if (*curf != '+' && *curf != '#' && *curf != *curn)
				break;
			if (*curf == '+') { // skip until we meet the next separator, or end of string
				char *nextpos = curn + 1;
				while (nextpos < curn_end && *nextpos != '/')
					nextpos = ++curn + 1;
			} else if (*curf == '#')
				curn = curn_end - 1;    // skip until end of string
			curf++;
			curn++;
		}

		return (curn == curn_end) && (*curf == '\0');
	}

	virtual bool connect(JETHERNET::NetSock &sock,
			const ClientConfig config) = 0;
	virtual bool reconnect() = 0;
	virtual void disconnect() = 0;
	virtual bool isConnected() = 0;

	virtual void registerMessageReceiver(MQTTMessageReceiver *recv) = 0;

	virtual void setMessageCallback(MessageCB func) = 0;
	virtual void setConnectCallback(ConnectCB func) = 0;

	virtual ~Client() {
	}
};

} /* namespace JMQTT */

#endif /* JMQTT_INCLUDE_CLIENT_HPP_ */
