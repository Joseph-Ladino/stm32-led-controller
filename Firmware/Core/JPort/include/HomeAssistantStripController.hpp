/*
 * HassLedController.hpp
 *
 *  Created on: Jul 12, 2024
 *      Author: user
 */

#ifndef JPORT_INCLUDE_HASSLEDCONTROLLER_HPP_
#define JPORT_INCLUDE_HASSLEDCONTROLLER_HPP_

#include <string>
#include <type_traits>
#include <ArduinoJson.h>
#include <DMAStrip.hpp>
#include "HomeAssistantDevice.hpp"
#include "NetConfig.hpp"
#include "globals.h"
#include "boardinfo.h"

using std::string;

template<typename StripBase>
class HomeAssistantStripController:
		public JMQTT::HomeAssistantDevice {
	ASSERT_STRIPBASE_IS_DERIVED(StripBase, typename StripBase::StripType);

public:
	using LEDStrip = StripBase;

private:
	JMQTT::Client *mqtt;
	LEDStrip &strip;
	JLED::WSStripState lastStripState;

	const string DeviceName, BaseTopic, StateTopic, DiscoveryTopic, CommandTopic, AvailabilityTopic;
public:

	inline void onMessage(JMQTT::Client &client, const JMQTT::Message &msg) final {
		USB_Printf("FROM LEDSTRIP: %s\n", ((string )msg).c_str());

		if (msg.topic == CommandTopic) {
			JsonDocument doc;
			deserializeJson(doc, msg.payload);

			strip.setPower(doc["state"] == "ON");

			if (doc.containsKey("color")) {
				auto color = doc["color"];
				if (!(color.containsKey("r") && color.containsKey("g") && color.containsKey("b"))) return;

				JLED::Color c;

				c.r = color["r"];
				c.g = color["g"];
				c.b = color["b"];

				strip.setAll(c);
			}

			if (doc.containsKey("brightness")) {
				strip.setBrightness(doc["brightness"]);
			}
		}

		publishState();
	}

	inline void publishState() final {
		JsonDocument doc;

		doc["state"] = lastStripState.powerOn ? "ON" : "OFF";
		doc["brightness"] = lastStripState.brightness;

		//TODO: modes and effects
		doc["effect"] = "none"; //sstrip.getEffect();
		doc["color_mode"] = "rgb"; //strip.getMode();

		JLED::Color color = lastStripState.color;

		doc["color"]["r"] = color.r;
		doc["color"]["g"] = color.g;
		doc["color"]["b"] = color.b;

		string stateJson;
		serializeJson(doc, stateJson);

		USB_Printf("\nPublishing LEDStrip state to MQTT broker:\n\t%s\n\n", stateJson.c_str());
		mqtt->publish( { StateTopic, stateJson, JMQTT::QOS::QOS2 });
	}

	inline void publishDiscovery() final {
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

//				if (netConf.mac != 0) {
//					char mac[18];
//					netConf.mac.cString(mac);
//					doc["device"]["cns"][0][0] = "mac";
//					doc["device"]["cns"][0][1] = (const char*) mac;
//				}
//
//				if (netConf.ip != 0) {
//					char ip[16];
//					netConf.ip.cString(ip);
//					doc["device"]["cns"][1][0] = "ip";
//					doc["device"]["cns"][1][1] = (const char*) ip;
//				}

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

		mqtt->publish(DiscoveryTopic, discoveryJson);
		mqtt->publish(AvailabilityTopic, "online");
	}

	inline void update() final {
		auto stripState = strip.getStripState();

		if (stripState != lastStripState) {
			lastStripState = stripState;

			publishState();
		}
	}

	inline void init(JMQTT::Client *client) final {
		client->subscribe(CommandTopic, this, JMQTT::QOS::QOS2);

		mqtt = client;

		publishDiscovery();
		publishState();
	}

	inline HomeAssistantStripController(LEDStrip &strip, std::string_view deviceName = "STM32_LEDStrip") : strip(strip), DeviceName(
			deviceName), BaseTopic("homeassistant/light/" + DeviceName), StateTopic(BaseTopic + "/state"), DiscoveryTopic(
			BaseTopic + "/config"), CommandTopic(StateTopic + "/set"), AvailabilityTopic(StateTopic + "/available") {
	}
};

#endif /* JPORT_INCLUDE_HASSLEDCONTROLLER_HPP_ */
