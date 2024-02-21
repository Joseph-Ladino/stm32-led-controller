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

// singleton
class W5500_Interface {
private:

	SPI_HandleTypeDef *spi;

	GPIO_TypeDef *chipSelectPort;
	uint16_t chipSelectPin;
	GPIO_TypeDef *resetPort;
	uint16_t resetPin;

#define ETH_BUFFER_SIZE 2048

	bool ipAssigned = false;

	static void readBuffer(uint8_t *buf, uint16_t len);
	static void writeBuffer(uint8_t *buf, uint16_t len);

	static uint8_t readByte();
	static void writeByte(uint8_t byte);

	static void enableChipSelect();
	static void disableChipSelect();

	static void CB_DHCP_IPAssigned();
	static void CB_DHCP_IPConflict();

	void initChip();
	void initDHCP();

	W5500_Interface();
//		virtual ~W5500_Interface();

public:
	// 1K should be enough, see https://forum.wiznet.io/t/topic/1612/2
	inline static volatile uint8_t DHCPBuffer[ETH_BUFFER_SIZE];
	// 1K seems to be enough for this buffer as well
	inline static volatile uint8_t DNSBuffer[ETH_BUFFER_SIZE];

	static W5500_Interface& instance();
	static void init(SPI_HandleTypeDef *spi,
			GPIO_TypeDef *chipSelectPort, uint16_t chipSelectPin,
			GPIO_TypeDef *resetPort, uint16_t resetPin);

	void triggerReset();
	wiz_NetInfo getNetInfo();
	void setNetInfo(wiz_NetInfo* netInfo);

	// prevent copies of singleton being generated
	W5500_Interface(W5500_Interface const&) = delete;
	void operator=(W5500_Interface const&) = delete;
};

}

#endif /* W5500INTERFACE_H_ */
