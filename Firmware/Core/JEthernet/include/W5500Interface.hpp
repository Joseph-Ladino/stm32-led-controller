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
#include "globals.h"
#include "socket.h"
#include "dhcp.h"
#include "dns.h"

namespace JOELIB {

#ifdef ETH_DEBUG
#define ETH_Printf(...) USB_Printf(__VA_ARGS__)
#else
#define ETH_Printf(...)
#endif



// singleton
class W5500Interface {
private:

	SPI_HandleTypeDef *spi;

	GPIO_TypeDef *chipSelectPort;
	uint16_t chipSelectPin;
	GPIO_TypeDef *resetPort;
	uint16_t resetPin;

#define ETH_BUFFER_SIZE 1024

	bool ipAssigned = false;

	static void readBuffer(uint8_t *buf, uint16_t len);
	static void writeBuffer(uint8_t *buf, uint16_t len);

	static uint8_t readByte();
	static void writeByte(uint8_t byte);

	static void enableChipSelect();
	static void disableChipSelect();

	static void CB_DHCP_IPAssigned();
	static void CB_DHCP_IPConflict();


	W5500Interface() = default;
//		virtual ~W5500_Interface();

public:
	enum class Socket : uint8_t {
		DHCP = 0,
		DNS,
		HTTP
	};

	bool initChip();
	bool initDHCP();
	bool initDNS();

	void printNetInfo();
	// 1K should be enough, see https://forum.wiznet.io/t/topic/1612/2
	inline static volatile uint8_t DHCPBuffer[ETH_BUFFER_SIZE];
	// 1K seems to be enough for this buffer as well
	inline static volatile uint8_t DNSBuffer[ETH_BUFFER_SIZE];

	static W5500Interface& instance();
	static bool init(SPI_HandleTypeDef *spi,
			GPIO_TypeDef *chipSelectPort, uint16_t chipSelectPin,
			GPIO_TypeDef *resetPort, uint16_t resetPin);

	void triggerReset();
	wiz_NetInfo getNetInfo();
	void setNetInfo(wiz_NetInfo* netInfo);

	// prevent copies of singleton being generated
	W5500Interface(W5500Interface const&) = delete;
	void operator=(W5500Interface const&) = delete;
};

}

#endif /* W5500INTERFACE_H_ */
