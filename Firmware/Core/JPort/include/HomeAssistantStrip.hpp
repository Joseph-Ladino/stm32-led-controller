/*
 * JLedstrip.hpp
 *
 *  Created on: Jul 9, 2024
 *      Author: user
 */

#ifndef JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_
#define JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_

#include <string>
#include <ArduinoJson.h>
#include "DMAStrip.hpp"
#include "HomeAssistantDevice.hpp"
#include "NetConfig.hpp"
#include "globals.h"
#include "boardinfo.h"

using std::string;
using JLED::Color;

template<uint16_t _NUM_PIXELS = 10, uint16_t _NUM_PIXELS_PER_DMA = 5, uint16_t _DMA_ARR = 79,
		typename DMA_BUF_TYPE = uint32_t>
class HomeAssistantStrip:
		public JLED::DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>,
		public JMQTT::HomeAssistantDevice {
	
private:
	using LEDBase = JLED::DMAStrip<_NUM_PIXELS,
	_NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>;

	const string DeviceName, BaseTopic, StateTopic, DiscoveryTopic, CommandTopic, AvailabilityTopic;

	JETHERNET::NetConfig netConf;

public:
	struct State {
		bool powerOn = true;
		Color staticColor = Color(0xFF00FF);
		uint8_t brightness = 255;
		std::string effect = "none";
		std::string colorMode = "rgb";
	};

	State state;

	inline void setPower(bool on) {
		state.powerOn = on;
		
		LEDBase::setPower(on);
	}
	
	inline void setBrightness(uint8_t brightness) {
		state.brightness = brightness;
		
		LEDBase::setBrightness(brightness);
	}
	
	// sets staticColor
	inline void setAll(uint32_t col) {
		state.staticColor = col;
		
		LEDBase::setAll(col);
	}
	
	// to be later implemented
//	void setEffect(std::string effect);
//	void setColorMode(std::string colorMode);
	
// MQTT
	inline void onMessage(JMQTT::Client &client, const JMQTT::Message &msg) final {
		USB_Printf("FROM LEDSTRIP: %s\n", ((string )msg).c_str());
		
		if (msg.topic == CommandTopic) {
			JsonDocument doc;
			deserializeJson(doc, msg.payload);
			
			setPower(doc["state"] == "ON");
			
			if (doc.containsKey("color")) {
				auto color = doc["color"];
				if (!(color.containsKey("r") && color.containsKey("g") && color.containsKey("b"))) return;
				
				Color c;
				
				c.r = color["r"];
				c.g = color["g"];
				c.b = color["b"];
				
				setAll(c);
			}
			
			if (doc.containsKey("brightness")) {
				setBrightness(doc["brightness"]);
			}
		}
		
		publishState(&client);
	}
	
	inline void publishState(JMQTT::Client *client) final {
		JsonDocument doc;
		
		doc["state"] = state.powerOn ? "ON" : "OFF";
		doc["brightness"] = state.brightness;
		doc["effect"] = state.effect;
		doc["color_mode"] = state.colorMode;
		doc["color"]["r"] = state.staticColor.r;
		doc["color"]["g"] = state.staticColor.g;
		doc["color"]["b"] = state.staticColor.b;
		
		string stateJson;
		serializeJson(doc, stateJson);
		
		USB_Printf("\nPublishing LEDStrip state to MQTT broker:\n\t%s\n\n", stateJson.c_str());
		client->publish(StateTopic, stateJson);
	}
	
	inline void publishDiscovery(JMQTT::Client *client) final {
		JsonDocument doc;
		
		// note: using abbreviated property names to reduce space
		// https://www.home-assistant.io/integrations/mqtt
		doc["name"] = nullptr;
//		doc["uniq_id"] = DeviceName + "_mqtt";
		doc["platform"] = "mqtt";
		doc["schema"] = "json"; // deprecated
				
		doc["uniq_id"] = "STM32LED_REV-" LED_CONTROLLER_SN;
		
		doc["device"]["name"] = DeviceName;
		doc["device"]["ids"] = doc["uniq_id"];
		doc["device"]["mf"] = "Joe Inc.";
		doc["device"]["mdl"] = "v4";
		doc["device"]["hw"] = LED_CONTROLLER_BD_VERSION;
		doc["device"]["sw"] = LED_CONTROLLER_FW_VERSION;
		doc["device"]["sn"] = LED_CONTROLLER_SN;
		
		if (netConf.mac != 0) {
			char mac[18];
			netConf.mac.cString(mac);
			doc["device"]["cns"][0][0] = "mac";
			doc["device"]["cns"][0][1] = (const char*) mac;
		}
		
		if (netConf.ip != 0) {
			char ip[16];
			netConf.ip.cString(ip);
			doc["device"]["cns"][1][0] = "ip";
			doc["device"]["cns"][1][1] = (const char*) ip;
		}
		
		doc["~"] = BaseTopic;
		doc["stat_t"] = "~/state";
		doc["cmd_t"] = "~/state/set";
		doc["avty_t"] = "~/state/available";
		
		doc["brightness"] = true;
		doc["clrm"] = true; // deprecated
		doc["sup_clrm"][0] = "rgb";
		
		// todo: configure effects
//		doc["effect"] = true;
		
//		for (auto i = 0u;
//				i < sizeof(LedstripEffects::EffectsLookup) / sizeof(String);
//				i++) {
//			doc["fx_list"][i] = LedstripEffects::EffectsLookup[i];
//		}
//		doc["fx_list"][0] = "none";
//
//		auto size = effects.size();
//		auto keys = effects.keys();
//
//		for (size_t i = 0; i < size; i++) {
//			doc["fx_list"][i + 1] = keys[i];
//		}
		
		string discoveryJson;
		serializeJson(doc, discoveryJson);
		
		USB_Printf("\nPublishing LEDStrip discovery to MQTT broker:\n\t%s\n\n", discoveryJson.c_str());
		
		client->publish(DiscoveryTopic, discoveryJson);
		client->publish(AvailabilityTopic, "online");
	}
	
	inline void init(JMQTT::Client *client) final {
//		this->client = client;
		client->subscribe(CommandTopic, this, JMQTT::QOS::QOS2);
		
		publishDiscovery(client);
		publishState(client);
	}
	
	inline HomeAssistantStrip(std::string_view deviceName = "STM32_LEDStrip", JETHERNET::NetConfig netConf = { }) : LEDBase::DMAStrip(), DeviceName(
			deviceName), BaseTopic("homeassistant/light/" + DeviceName), // TODO: change base topic level to "homeassistant" instead of "test"
	StateTopic(BaseTopic + "/state"), DiscoveryTopic(BaseTopic + "/config"), CommandTopic(StateTopic + "/set"), AvailabilityTopic(
			StateTopic + "/available"), netConf(netConf) {
	}
}
;

#endif /* JPORT_INCLUDE_HOMEASSISTANTSTRIP_HPP_ */
