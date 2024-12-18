/*
 * W5500Socket.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_W5500SOCKET_HPP_
#define JETHERNET_INCLUDE_W5500SOCKET_HPP_

#include <NetSock.hpp>
#include <cstdint>

namespace JETHERNET {

class W5500HC;
class W5500Socket : public NetSock {
private:
	W5500HC* ethHC;
	int socketNum = -1;
	SockConnectMode connectMode;

public:
	bool open(NetIP ip, uint16_t port, SockConnectMode mode) override;
	bool close() override;
	bool connectTCP(NetIP ip, uint16_t port) override;
	bool disconnect() override;
	int write(uint8_t* dataIn, uint16_t len) override;
	int read(uint8_t* dataOut, uint16_t len) override;

	uint8_t getWizSocketStatus() const;
	int getSocketNum() const;
	bool isConnected() const;
	operator bool() const;

	friend class W5500HC;

	W5500Socket();
	W5500Socket(int sn);
	~W5500Socket() = default;
};

} /* namespace JETHERNET */

#endif /* JETHERNET_INCLUDE_W5500SOCKET_HPP_ */
