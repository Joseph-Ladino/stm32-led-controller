/*
 * W5500Socket.cpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#include <W5500Socket.hpp>
#include "socket.h"

static int8_t wizSocket(uint8_t sn, uint8_t protocol, uint16_t port,
		uint8_t flag) {
	return socket(sn, protocol, port, flag);
}

static int8_t wizConnect(uint8_t sn, uint8_t *addr, uint16_t port) {
	return connect(sn, addr, port);
}

static int8_t wizDisconnect(uint8_t sn) {
	return disconnect(sn);
}

static int8_t wizClose(uint8_t sn) {
	return close(sn);
}

namespace JETHERNET {

bool W5500Socket::open(EthernetIP ip, uint16_t port, EthernetConnectMode mode) {
	connectMode = mode;
	return wizSocket(socketNum, (uint8_t) mode, port, 0) == socketNum;
}

bool W5500Socket::close() {
	return wizClose(socketNum) == SOCK_OK;
}

bool W5500Socket::connectTCP(EthernetIP ip, uint16_t port) {

	if (!isConnected() && !open(ip, port, EthernetConnectMode::TCP))
		return false;

	if (connectMode != EthernetConnectMode::TCP)
		return false;

	return wizConnect(socketNum, ip.raw, port);
}

bool W5500Socket::disconnect() {
	return wizDisconnect(socketNum) == SOCK_OK;
}

bool W5500Socket::write(uint8_t *dataIn, uint16_t len) {
	if (isConnected())
		return send(socketNum, dataIn, len) == len;

	return false;
}

bool W5500Socket::read(uint8_t *dataOut, uint16_t len) {
	if (isConnected() && getSn_RX_RSR(socketNum) > 0)
		return recv(socketNum, dataOut, len) == len;

	return false;
}

int W5500Socket::getSocketNum() const {
	return socketNum;
}

bool W5500Socket::isConnected() const {
	auto rc = getSn_SR(socketNum);
	return rc != SOCK_CLOSE_WAIT && rc != SOCK_CLOSED;
}

W5500Socket::operator bool() const {
	return isConnected();
}

W5500Socket::W5500Socket() {
}

W5500Socket::W5500Socket(int sn) :
		socketNum(sn) {
}

} /* namespace JETHERNET */
