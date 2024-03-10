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

namespace JETHERNET {

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
