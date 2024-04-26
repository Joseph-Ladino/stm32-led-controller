/*
 * W500HC.hpp
 *
 *  Created on: Feb 22, 2024
 *      Author: DRTNT
 */

#ifndef JETHERNET_INCLUDE_W5500HC_HPP_
#define JETHERNET_INCLUDE_W5500HC_HPP_

#include <NetIP.hpp>
#include "W5500Interface.hpp"
#include "W5500Socket.hpp"
#include "EthernetHC.hpp"

namespace JETHERNET {

class W5500HC : public EthernetHC {
protected:
	bool dhcpSuccess = false;
	bool initSuccess = false;
	bool phyTimeout = false;

	W5500Socket sockets[8];

private:
	W5500Config hwConfig;
	W5500Interface &hw = W5500Interface::instance();
	NetIP dnsAddress;
	bool dhcpEnabled;

public:
	void setConfig(NetConfig &netConfig) override;
	NetConfig getConfig() override;

	void msTick() override;

	bool enableDHCP(uint16_t timeoutMs) override;
	bool enableDNS(NetIP dns = 0) override;
	NetIP domainToIP(const char * domain, uint16_t timeoutMs) override;

	bool phyLinkStatus() override;
	bool waitForLink(uint16_t timeoutMs) override;
	void softReset();
	bool init(void*) override;

	W5500Socket& getFreeSocket() ;

	~W5500HC();
};

} /* namespace JOELIB */

#endif /* JETHERNET_INCLUDE_W5500HC_HPP_ */
