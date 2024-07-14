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
	auto rc = client.publish(msg.topic.data(), (void*) msg.payload.data(), (size_t) msg.payload.length(),
			(MQTT::QoS) msg.qos);
	
	return rc == MQTT::SUCCESS;
}

bool PAHOClient::subscribe(string_view topic, QOS qos) {
	auto rc = client.subscribe(topic.data(), (MQTT::QoS) qos, { this, &PAHOClient::pahoMessageReceived });
	
	client.setMessageHandler(topic.data(), { }); // delete specific handler from embedded client
			
	return rc == MQTT::SUCCESS;
}

bool PAHOClient::subscribe(string_view topic, MQTTMessageReceiver *recv, QOS qos) {
	if (!subscribe(topic, qos)) return false;
	
	specificReceivers.emplace_back(topic, recv);
	
	return true;
}

bool PAHOClient::connect(NetSock &sock, const ClientConfig config) {
	net.sock = &sock;
	net.conf = config;
	
	return connect();
}

bool PAHOClient::connect() {
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	
	data.MQTTVersion = (uint8_t) net.conf.version;
	data.clientID.cstring = (char*) net.conf.clientName.data();
	data.username.cstring = (char*) net.conf.username.data();
	data.password.cstring = (char*) net.conf.password.data();
	
	// connect socket to broker
	if (!net.sock->isConnected() && !net.sock->connectTCP(net.conf.brokerIP, net.conf.brokerPort)) {
		return false;
	}
	
	// connect client (through socket) to broker
	if (!client.connect(data) == MQTT::SUCCESS) return false;
	
	client.setDefaultMessageHandler( { this, &PAHOClient::pahoMessageReceived });
	
	if (onConnect) onConnect(*this);
	
	return true;
}

void PAHOClient::disconnect() {
	client.disconnect();
	net.sock->disconnect();
	net.sock->close();
}

bool PAHOClient::reconnect() {
	return connect();
}

bool PAHOClient::reconnect(NetSock &sock) {
	net.sock = &sock;
	
	return connect();
}

bool PAHOClient::update(uint16_t timeoutMs) {
	CountdownTimer t(timeoutMs);
	auto rc = client.yield(timeoutMs);
	
	if (rc != MQTT::SUCCESS || !net.sock->isConnected()) {
		disconnect();
		return false;
	}
	
	return true;
}

bool PAHOClient::isConnected() {
	return client.isConnected();
}

PAHOClient::PAHOClient(const uint16_t numReceivers) : client(net) {
	genericReceivers.reserve(numReceivers);
	specificReceivers.reserve(numReceivers);
}

PAHOClient::~PAHOClient() {
}

void PAHOClient::registerMessageReceiver(MQTTMessageReceiver *recv) {
	if (recv == nullptr) return;
	genericReceivers.push_back(recv);
}

void PAHOClient::setMessageCallback(PAHOClient::MessageCB func) {
	onMessage = func;
}

void PAHOClient::setConnectCallback(PAHOClient::ConnectCB func) {
	onConnect = func;
}
#include "globals.h"
void PAHOClient::pahoMessageReceived(MQTT::MessageData &data) {
	
	if (!(onMessage || genericReceivers.empty() || specificReceivers.empty())) return;
	
	JMQTT::Message msg;
	msg.topic = string(data.topicName.lenstring.data, data.topicName.lenstring.len);
	msg.payload = string((char*) data.message.payload, data.message.payloadlen);
	msg.qos = (JMQTT::QOS) data.message.qos;
	
	onMessage(*this, msg);
	
	for (auto &recv : genericReceivers) {
		recv->onMessage(*this, msg);
	}
	for (auto &pair : specificReceivers) {
		if (isTopicMatched(msg.topic, pair.first)) {
			pair.second->onMessage(*this, msg);
		}
	}
//	msg.qos = data
}

} /* namespace JMQTT */
