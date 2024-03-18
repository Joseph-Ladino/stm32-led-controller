/*
 * MQTTInterface.cpp
 *
 *  Created on: Mar 10, 2024
 *      Author: user
 */

#include "MQTTInterface.hpp"
#include "socket.h"

namespace JMQTT {

/* wrappers for socket API */

static int8_t wizSocket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag) {
	return socket(sn, protocol, port, flag);
}

static int8_t wizConnect(uint8_t sn, uint8_t *addr, uint16_t port) {
	return connect(sn, addr, port);
}

static int8_t wizDisconnect(uint8_t sn) {
	return disconnect(sn);
}

/* MQTT NETWORK CLASS */

int W5500MqttNetwork::connect(const char *hostname, uint16_t port) {
	// TODO: initiate DNS in W5500HC and implement method
	return -1;
}

int W5500MqttNetwork::connect(uint8_t *ip, uint16_t port) {
	if (wizSocket(socketNum, Sn_MR_TCP, 12345, 0) != socketNum)
		return SOCK_ERROR;

	if (wizConnect(socketNum, ip, port) != SOCK_OK)
		return SOCK_ERROR;

	return SOCK_OK;
}

void W5500MqttNetwork::disconnect() {
	wizDisconnect(socketNum);
}

int W5500MqttNetwork::read(unsigned char *buffer, int len, int timeout_ms) {
	if (isConnected() && getSn_RX_RSR(socketNum) > 0)
		return recv(socketNum, buffer, len);

	return SOCK_ERROR;
}

int W5500MqttNetwork::write(unsigned char *buffer, int len, int timeout_ms) {
	if (isConnected())
		return send(socketNum, buffer, len);

	return SOCK_ERROR;
}

bool W5500MqttNetwork::isConnected() {
	return getSn_SR(socketNum) == SOCK_ESTABLISHED;
}

void W5500MqttNetwork::setSocket(int socketNum) {
	if (isConnected())
		disconnect();
	this->socketNum = socketNum;
}

W5500MqttNetwork::W5500MqttNetwork(int socketNum) :
		socketNum(socketNum) {
}

W5500MqttNetwork::W5500MqttNetwork() :
		socketNum(0) {
}

}

