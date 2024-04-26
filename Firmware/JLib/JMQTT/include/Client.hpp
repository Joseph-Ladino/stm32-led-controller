/*
 * MQTTClient.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: user
 */

#ifndef JMQTT_INCLUDE_CLIENT_HPP_
#define JMQTT_INCLUDE_CLIENT_HPP_

#include <NetSock.hpp>
#include <cstdint>
#include <string>
#include <functional>

using std::string_view;
using std::string;

namespace JMQTT {

enum class QOS : uint8_t {
	QOS0 = 0,
	QOS1,
	QOS2
};

struct Message {
	string topic = "";
	string payload = "";
	QOS qos = QOS::QOS0;
};

enum class MQTTVersion : uint8_t {
	MQTT3 = 3,
	MQTT311 = 4,
	MQTT5 = 5,
};

struct ClientConfig {
	MQTTVersion version = MQTTVersion::MQTT311;
	JETHERNET::NetIP brokerIP = 0;
	uint16_t brokerPort = 0;
	string clientName = "JMQTTClient";
	string username = "";
	string password = "";
};

class Client {
public:

	using ConnectCB = std::function<void(Client&)>;
	using MessageCB = std::function<void(Client&, Message)>;

	virtual bool publish(Message msg) = 0;
	virtual bool subscribe(string_view topic, QOS qos = QOS::QOS0) = 0;
	virtual bool update(uint16_t timeoutMs) = 0;

	virtual bool connect(JETHERNET::NetSock &sock, const ClientConfig config) = 0;
	virtual bool reconnect() = 0;
	virtual void disconnect() = 0;
	virtual bool isConnected() = 0;

	virtual void setMessageCallback(MessageCB func) = 0;
	virtual void setConnectCallback(ConnectCB func) = 0;

	virtual ~Client() {}
//	Client(EthernetSOCK& sock, ClientConfig config);
};

} /* namespace JMQTT */

#endif /* JMQTT_INCLUDE_CLIENT_HPP_ */
