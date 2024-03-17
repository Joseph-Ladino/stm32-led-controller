/*
 * EthernetSOCK.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_ETHERNETSOCK_HPP_
#define JETHERNET_INCLUDE_ETHERNETSOCK_HPP_

#include <cstdint>
#include "EthernetIP.hpp"

namespace JETHERNET {

enum class EthernetConnectMode : uint8_t {
	TCP = 1,
	UDP = 2,
	RAWIP = 3,
	RAWMAC = 4
};

// forward declare to circular dependencies
class EthernetHC;
class EthernetSOCK {
protected:
//	EthernetHC* ethHC;

public:
	virtual bool open(EthernetIP ip, uint16_t port, EthernetConnectMode mode) = 0;
	virtual bool close() = 0;
	virtual bool connectTCP(EthernetIP ip, uint16_t port) = 0;
	virtual bool disconnect() = 0;
	virtual bool write(uint8_t* dataIn, uint16_t len) = 0;
	virtual bool read(uint8_t* dataOut, uint16_t len) = 0;

	// return true if socket is open and false if closed
	virtual bool isConnected() const = 0;

	virtual ~EthernetSOCK() {}
};

}

#endif /* JETHERNET_INCLUDE_ETHERNETSOCK_HPP_ */
