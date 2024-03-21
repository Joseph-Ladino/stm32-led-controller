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

bool PAHOClient::publish(Message msg) {
	auto rc = client.publish(msg.topic.data(), (void*) msg.payload.data(),
			(size_t) msg.payload.length(), (MQTT::QoS) msg.qos);

	return rc == MQTT::SUCCESS;
}

bool PAHOClient::subscribe(string_view topic, QOS qos) {
	auto rc = client.subscribe(topic.data(), (MQTT::QoS)qos, {this, &PAHOClient::pahoMessageReceived});

	return rc == MQTT::SUCCESS;
}

bool PAHOClient::connect(EthernetSOCK &sock, const ClientConfig config) {
	net.sock = &sock;
	net.conf = config;

	return connect();
}

bool PAHOClient::connect() {
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

		data.MQTTVersion = (uint8_t) net.conf.version;
		data.clientID.cstring = (char*)net.conf.clientName.data();
		data.username.cstring = (char*)net.conf.username.data();
		data.password.cstring = (char*)net.conf.password.data();

		bool connected = client.connect(data) == MQTT::SUCCESS;
		if (!connected)
			return false;

		client.setDefaultMessageHandler(this, &PAHOClient::pahoMessageReceived);

		if(onConnectCallback)
			onConnectCallback(*this);

		return true;
}

void PAHOClient::disconnect() {
	client.disconnect();
}

bool PAHOClient::reconnect() {
	return connect();
}

bool PAHOClient::update(uint16_t timeoutMs) {
	CountdownTimer t(timeoutMs);
	auto rc = client.yield(timeoutMs);

	if(rc != MQTT::SUCCESS) {
		disconnect();
		return false;
	}

	return true;
}

bool PAHOClient::isConnected() {
	return client.isConnected();
}

PAHOClient::PAHOClient() :
		client(net) {
}

PAHOClient::~PAHOClient() {}

void PAHOClient::setMessageCallback(PAHOClient::messageCallbackFunc func) {
	onMessageCallback = func;
}

void PAHOClient::setConnectCallback(PAHOClient::connectCallbackFunc func) {
	onConnectCallback = func;
}

void PAHOClient::pahoMessageReceived(MQTT::MessageData &data) {

	if(!onMessageCallback) return;

	JMQTT::Message msg;
	msg.topic = string(data.topicName.lenstring.data, data.topicName.lenstring.len);
	msg.payload = string((char*)data.message.payload, data.message.payloadlen);
	msg.qos = (JMQTT::QOS) data.message.qos;

	onMessageCallback(msg);
//	msg.qos = data
}


} /* namespace JMQTT */
