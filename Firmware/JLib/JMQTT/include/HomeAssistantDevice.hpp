/*
 * MQTTLedstripDevice.hpp
 *
 *  Created on: Jul 9, 2024
 *      Author: user
 */

#ifndef JMQTT_INCLUDE_HOMEASSISTANTDEVICE_HPP_
#define JMQTT_INCLUDE_HOMEASSISTANTDEVICE_HPP_

#include <string>
#include "Client.hpp"

namespace JMQTT {

class HomeAssistantDevice: public MQTTMessageReceiver {
	
	virtual void onMessage(Client&, const Message&) = 0;
	virtual void publishState(JMQTT::Client *client) = 0;
	virtual void publishDiscovery(JMQTT::Client *client) = 0;
	virtual void init(Client*) = 0;
};
}

#endif /* JMQTT_INCLUDE_HOMEASSISTANTDEVICE_HPP_ */