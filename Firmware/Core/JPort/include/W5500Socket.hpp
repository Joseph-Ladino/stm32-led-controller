/*
 * W5500Socket.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_W5500SOCKET_HPP_
#define JETHERNET_INCLUDE_W5500SOCKET_HPP_

#include "stdint.h"
#include "EthernetSOCK.hpp"

namespace JETHERNET {

class W5500HC;
class W5500Socket : public EthernetSOCK {
private:
	W5500HC* ethHC;
	int socketNum;

public:
	void open() override;
	void close(void* sock) override;
	bool write(uint8_t* dataIn, uint16_t len) override;
	bool read(uint8_t* dataOut, uint16_t len) override;

	friend class W5500HC;

	W5500Socket();
	W5500Socket(int sn);
	~W5500Socket() = default;
};

} /* namespace JETHERNET */

#endif /* JETHERNET_INCLUDE_W5500SOCKET_HPP_ */
