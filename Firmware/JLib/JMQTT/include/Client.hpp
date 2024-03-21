/*
 * MQTTClient.hpp
 *
 *  Created on: Mar 17, 2024
 *      Author: user
 */

#ifndef JMQTT_INCLUDE_CLIENT_HPP_
#define JMQTT_INCLUDE_CLIENT_HPP_

#include <cstdint>
#include <string_view>
#include <string>
#include "EthernetSOCK.hpp"

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
	string clientName = "JMQTTClient";
	string username = "";
	string password = "";
};

class Client {
public:
	virtual void publish(const char * topic, const char * message, QOS qos = QOS::QOS0) = 0;
	virtual void subscribe(const char * topic, QOS qos = QOS::QOS0) = 0;
	virtual bool connect(JETHERNET::EthernetSOCK& sock, ClientConfig config) = 0;
	virtual void disconnect() = 0;

	virtual void messageReceived(Message* msg) = 0;

	virtual ~Client() {}
//	Client(EthernetSOCK& sock, ClientConfig config);
};

} /* namespace JMQTT */

#endif /* JMQTT_INCLUDE_CLIENT_HPP_ */
