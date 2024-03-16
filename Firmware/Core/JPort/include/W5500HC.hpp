/*
 * W500HC.hpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#ifndef JETHERNET_INCLUDE_W5500HC_HPP_
#define JETHERNET_INCLUDE_W5500HC_HPP_

#include "W5500Interface.hpp"
#include "EthernetHC.hpp"
#include "W5500Socket.hpp"
#include "EthernetIP.hpp"

namespace JETHERNET {

class W5500HC : public EthernetHC {
private:
	W5500Config hwConfig;
	W5500Interface &hw = W5500Interface::instance();
	EthernetIP dnsAddress;
	bool dhcpEnabled;

public:
	void oneSecondPassed() override;

	void setConfig(NetConfig &netConfig) override;
	NetConfig getConfig() override;

	bool enableDHCP() override;
	bool phyLinkStatus() override;
	bool waitForPhyLink() override;
	bool init(void*) override;

	W5500Socket& getFreeSocket() ;

	~W5500HC();
};

} /* namespace JOELIB */

#endif /* JETHERNET_INCLUDE_W5500HC_HPP_ */
