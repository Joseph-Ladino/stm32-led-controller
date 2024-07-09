/*
 * EthernetSOCK.hpp
 *
 *  Created on: Mar 14, 2024
 *      Author: user
 */

#ifndef JETHERNET_INCLUDE_NETSOCK_HPP_
#define JETHERNET_INCLUDE_NETSOCK_HPP_

#include <NetIP.hpp>
#include <cstdint>

namespace JETHERNET {

/**
 * @brief Connection modes for socket
 * @see NetSock
 */
enum class SockConnectMode : uint8_t {
	TCP = 1,
	UDP = 2,
	RAWIP = 3,
	RAWMAC = 4
};

// forward declare to circular dependencies
class EthernetHC;

/**
 * @brief Virtual class designed to represent a network socket
 * @see SockConnectMode
 * @see NetIP
 */
class NetSock {
protected:

public:
	/**
	 * @brief
	 * @param ip destination IP
	 * @param port destination port
	 * @param mode connection mode
	 * @return true if socket successfully opened
	 */
	virtual bool open(NetIP ip, uint16_t port, SockConnectMode mode) = 0;

	/**
	 * @brief close socket
	 * @return true if socket successfully closed
	 */
	virtual bool close() = 0;

	/**
	 * @brief connect to TCP server
	 * @param ip destination IP
	 * @param port destination port
	 * @return true if successfully connected
	 * @note will attempt to open socket if not open when called
	 */
	virtual bool connectTCP(NetIP ip, uint16_t port) = 0;

	/**
	 * @brief disconnect socket
	 * @return true if socket successfully disconnected from server
	 */
	virtual bool disconnect() = 0;

	/**
	 * @brief send data
	 * @param dataIn data to be written
	 * @param len number of bytes to be written
	 * @return number of bytes written
	 */
	virtual int write(uint8_t *dataIn, uint16_t len) = 0;

	/**
	 * @brief receive data
	 * @param dataOut buffer for data to be received into
	 * @param len number of bytes to be received
	 * @return number of bytes read
	 */
	virtual int read(uint8_t *dataOut, uint16_t len) = 0;

	// return true if socket is open and false if closed
	/**
	 * @brief check if socket is connected/open
	 * @return true if socket is open or connected
	 * @warning misleading name, will probably correct in the future
	 */
	virtual bool isConnected() const = 0;

	virtual ~NetSock() {
	}
};

}

#endif /* JETHERNET_INCLUDE_NETSOCK_HPP_ */
