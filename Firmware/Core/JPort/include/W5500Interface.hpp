/*
 * W5500Interface.h
 *
 *  Created on: Feb 17, 2024
 *      Author: Joseph Ladino
 *      @see <a href="https://github.com/afiskon/stm32-w5500/blob/master/Src/main.c">reference implementation</a>
 */

#ifndef W5500INTERFACE_H_
#define W5500INTERFACE_H_

#include "stm32g0xx_hal.h"
#include "EthernetHC.hpp"
#include "globals.h"
#include "socket.h"
#include "dhcp.h"
#include "dns.h"

namespace JETHERNET {

#ifdef ETH_DEBUG
#define ETH_Printf(...) USB_Printf(__VA_ARGS__)
#else
#define ETH_Printf(...)
#endif

#ifndef ETH_BUFFER_SIZE
#define ETH_BUFFER_SIZE 1 // kb
#endif

struct W5500Config {
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef *chipSelectPort;
	uint16_t chipSelectPin;
	GPIO_TypeDef *resetPort;
	uint16_t resetPin;
};

class W5500Socket;
// singleton
class W5500Interface {
private:
	inline static W5500Config config;
	inline static bool ipAssigned = false;
	inline static bool dhcpEnabled = false;

	W5500Interface() = default;
//		virtual ~W5500_Interface();

public:

	friend class W5500HC;

	friend void readBuffer(uint8_t *buf, uint16_t len);
	friend void writeBuffer(uint8_t *buf, uint16_t len);

	friend uint8_t readByte();
	friend void writeByte(uint8_t byte);

	friend void enableChipSelect();
	friend void disableChipSelect();

	friend void CB_DHCP_IPAssigned();
	friend void CB_DHCP_IPConflict();

	bool initChip();
	bool initDHCP(W5500Socket& sock, NetConfig* conf, uint16_t timeoutMs);
	bool initDNS();

	static W5500Interface& instance();
	static bool init(W5500Config config);

	void triggerReset();

	// prevent copies of singleton being generated
	W5500Interface(W5500Interface const&) = delete;
	void operator=(W5500Interface const&) = delete;
};

}

#endif /* W5500INTERFACE_H_ */
