/*
 * JLedstrip.hpp
 *
 *  Created on: Jul 9, 2024
 *      Author: user
 */

#ifndef JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_
#define JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_

#include "DMAStrip.hpp"
#include "Client.hpp"
#include "HomeAssistantDevice.hpp"
#include "globals.h"

// what the fuck
template<typename LEDSTRIP>
class HomeAssistantStrip: public LEDSTRIP, public JMQTT::HomeAssistantDevice {

private:
	JMQTT::Client *client;
public:

	// MQTT
	inline void onMessage(JMQTT::Client &client,
			const JMQTT::Message &msg) final {
		USB_Printf("FROM LEDSTRIP: %s\n", ((string )msg).data());
	}

	inline void publishState() final {
		client->publish( { "test/ledstrip", "hello I am test" });
	}

	inline void publishDiscovery() final {

	}

	inline void attachClient(JMQTT::Client *client) final {
		this->client = client;
		client->subscribe("ledstrip/#", this, JMQTT::QOS::QOS2);
	}
}
;

#endif /* JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_ */
