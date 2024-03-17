/*
 * W5500Socket.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_W5500SOCKET_HPP_
#define JETHERNET_INCLUDE_W5500SOCKET_HPP_

#include <cstdint>
#include "EthernetSOCK.hpp"

namespace JETHERNET {

class W5500HC;
class W5500Socket : public EthernetSOCK {
private:
	W5500HC* ethHC;
	int socketNum = -1;
	EthernetConnectMode connectMode;

public:
	bool open(EthernetIP ip, uint16_t port, EthernetConnectMode mode) override;
	bool close() override;
	bool connectTCP(EthernetIP ip, uint16_t port) override;
	bool disconnect() override;
	bool write(uint8_t* dataIn, uint16_t len) override;
	bool read(uint8_t* dataOut, uint16_t len) override;

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
