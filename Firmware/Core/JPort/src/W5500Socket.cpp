/*
 * W5500Socket.cpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#include <W5500Socket.hpp>
#include "socket.h"

int8_t wizSocket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag) {
	return socket(sn, protocol, port, flag);
}

int8_t wizConnect(uint8_t sn, uint8_t *addr, uint16_t port) {
	return connect(sn, addr, port);
}

int8_t wizDisconnect(uint8_t sn) {
	return disconnect(sn);
}



namespace JETHERNET {

void W5500Socket::open() {

}

void W5500Socket::close(void *sock) {

}

bool W5500Socket::write(uint8_t* dataIn, uint16_t len) {

	return false;
}

bool W5500Socket::read(uint8_t* dataOut, uint16_t len) {

	return false;
}

W5500Socket::W5500Socket() : socketNum(0) {
}

W5500Socket::W5500Socket(int sn) : socketNum(sn) {
}

} /* namespace JETHERNET */
