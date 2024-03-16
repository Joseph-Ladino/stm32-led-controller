/*
 * EthernetSOCK.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_ETHERNETSOCK_HPP_
#define JETHERNET_INCLUDE_ETHERNETSOCK_HPP_

#include <stdint.h>
#include <EthernetIP.hpp>

namespace JETHERNET {

// forward declare to circular dependencies
class EthernetHC;
class EthernetSOCK {
protected:
	EthernetHC* ethHC;

public:
	virtual void open() = 0;
	virtual void close(void* sock) = 0;
//	virtual void connect();
//	virtual void disconnect();
	virtual bool write(uint8_t* dataIn, uint16_t len) = 0;
	virtual bool read(uint8_t* dataOut, uint16_t len) = 0;

	virtual ~EthernetSOCK() {}
};

}

#endif /* JETHERNET_INCLUDE_ETHERNETSOCK_HPP_ */
