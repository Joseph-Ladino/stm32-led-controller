/*
 * W5500Client.cpp
 *
 *  Created on: Mar 17, 2024
 *      Author: user
 */

#include <cstring>
#include "PAHOClient.hpp"

using namespace JETHERNET;

namespace JMQTT {

void PAHOClient::publish(Message msg) {
	client.publish(msg.topic.data(), (void*) msg.payload.data(),
			(size_t) msg.payload.length(), (MQTT::QoS) msg.qos);
}

void PAHOClient::subscribe(string_view topic, QOS qos) {

}

bool PAHOClient::connect(EthernetSOCK &sock, ClientConfig config) {
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

	net.sock = &sock;

	data.MQTTVersion = (uint8_t) config.version;
	data.clientID.cstring = (char*) config.clientName;
	data.username.cstring = (char*) config.username;
	data.password.cstring = (char*) config.password;

	bool connected = client.connect(data) == MQTT::SUCCESS;
	if (!connected)
		return false;

	client.setDefaultMessageHandler(this, &PAHOClient::pahoMessageReceived);

	return true;
}

void PAHOClient::disconnect() {
}

void PAHOClient::messageReceived(Message *msg) {
}

PAHOClient::PAHOClient() :
		client(net) {
}

PAHOClient::~PAHOClient() {
}

void PAHOClient::setMessageHandler(const std::function<void(Message)> &func) {
	handler = func;
}

void PAHOClient::pahoMessageReceived(MQTT::MessageData &data) {

	JMQTT::Message msg;
	msg.topic = data.topicName.cstring;
	msg.payload = (char*) data.message.payload;
	msg.qos = (JMQTT::QOS) data.message.qos;

	handler(msg);
//	msg.qos = data
}

} /* namespace JMQTT */
