/*
 * W500HC.hpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#ifndef JETHERNET_INCLUDE_W5500HC_HPP_
#define JETHERNET_INCLUDE_W5500HC_HPP_

#include "stm32g0xx_hal.h"
#include "W5500Interface.hpp"
#include "EthernetHC.hpp"

namespace JOELIB {

struct W5500Config {
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef *chipSelectPort;
	uint16_t chipSelectPin;
	GPIO_TypeDef *resetPort;
	uint16_t resetPin;
};

class W5500HC: public EthernetHC {
private:
	W5500Config hwConfig;
	W5500Interface &hw = W5500Interface::instance();

public:
	void oneSecondPassed() override;

	void setConfig(NetConfig &netConfig) override;
	NetConfig getConfig() override;

	bool enableDHCP() override;
	bool phyLinkStatus() override;
	bool waitForPhyLink() override;
	bool init(void*) override;

	~W5500HC();
};

} /* namespace JOELIB */

#endif /* JETHERNET_INCLUDE_W5500HC_HPP_ */
