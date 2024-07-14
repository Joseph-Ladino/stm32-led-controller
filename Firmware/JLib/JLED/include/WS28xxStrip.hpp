/*
 * WS28xxStrip.hpp
 *
 *  Created on: Jul 13, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_WS28XXSTRIP_HPP_
#define JLED_INCLUDE_WS28XXSTRIP_HPP_

#include "StripTypes.hpp"

namespace JLED {

struct WSStripState {
	bool powerOn = true;
	uint8_t brightness = 255;
	Color color = 0x00;
	// TODO: add effect and current mode
};

/**
 * @brief Basic control class for an LED strip
 * @tparam StripType Strip characteristics (must be derived from JLED::StripType)
 */
template<typename StripType>
class WS28xxStrip {
public:
	ASSERT_STRIPTYPE_IS_DERIVED(StripType);
	using PhysicalPowerCB = void(*)(bool);

private:
	const uint16_t numPixels;
	WSStripState state;
	PhysicalPowerCB setPhysicalPower = nullptr;

protected:
	uint32_t *rawBuffer = nullptr, *fxBuffer = nullptr;

public:

	struct Iterator;

	/**
	 * @brief Set or read an element in the raw buffer
	 * @param index
	 * @return
	 */
	inline uint32_t& operator[](uint16_t index) {
		return rawBuffer[index];
	}
	
	/**
	 * @brief Read an element in the raw buffer
	 * @param index
	 * @return
	 */
	inline const uint32_t& operator[](uint16_t index) const {
		return rawBuffer[index];
	}
	
	/**
	 * @brief Set entire strip to color
	 * @param color 0xWWRRGGBB
	 */
	inline void setAll(uint32_t color) {
		for (uint16_t i = 0; i < numPixels; i++) {
			rawBuffer[i] = color;
		}
	}
	
	/**
	 * @brief Update power state
	 * @param powerOn true = on, false = off
	 */
	inline void setPower(bool powerOn) {
		state.powerOn = powerOn;
		
		if (setPhysicalPower) {
			setPhysicalPower(powerOn);
		}
		
		if (!powerOn) {
			memset(fxBuffer, 0, sizeof(fxBuffer[0]) * numPixels);
		}
	}
	
	/**
	 * @brief Retrieve power state
	 * @return true = on, false = off
	 */
	inline bool getPower() const {
		return state.powerOn;
	}
	
	/**
	 * @brief Set a callback to allow disconnecting power lines to LED strip. This reduces idle current when the strip is set to "off"
	 * @param cb function to toggle on/off a relay/mosfet/load switch
	 * @note This is an optional feature
	 */
	inline void setPhysicalPowerCB(PhysicalPowerCB cb) {
		setPhysicalPower = cb;
	}
	
	/**
	 * @brief  Set global strip brightness scalar
	 * @param brightness 0 = off, 255 = max brightness
	 */
	inline void setBrightness(uint8_t brightness) {
		state.brightness = brightness;
	}
	
	/**
	 * @brief Retrieve global strip brightness
	 * @return 0 = off, 255 = max brightness
	 */
	inline uint8_t getBrightness() const {
		return state.brightness;
	}
	
	inline void setEffect(); // TODO: Implement Effects/Animations class(es)
	inline void getEffect() const;

	inline void setMode(); // TODO: Implement mode (related to effects/animations)
	inline void getMode() const;

	/**
	 * @brief Retrieve number of pixels being controller
	 * @return number of pixels being controller
	 */
	inline uint16_t getNumPixels() const {
		return numPixels;
	}
	
	/**
	 * @brief Set rawBuffer and fxBuffer after construction
	 * @param rawBuffer array of raw color data
	 * @param fxBuffer array of color data with brightness and other effects applied
	 * @note both buffers should be contain numPixels elements
	 */
	inline void setPixelBuffers(uint32_t *rawBuffer, uint32_t *fxBuffer) {
		this->rawBuffer = rawBuffer;
		this->fxBuffer = fxBuffer;
	}
	
	/**
	 * @brief Transfers color data from raw buffer to fx buffer with brightness and effects applied
	 * The process of transferring color data to the physical LED strip is to be defined in a derived class. The derived class should still call this function before transferring data
	 */
	inline void display() {
		if (!state.powerOn) return;
		
		// TODO: filters/effects/animations
		for (uint16_t i = 0; i < numPixels; i++) {
			fxBuffer[i] = StripType::getOrderedBytes(rawBuffer[i]) * state.brightness / 255;
		}
	}
	
	inline const Iterator begin() {
		return beginRawBuffer();
	}
	inline const Iterator end() {
		return endRawBuffer();
	}
	
	inline const Iterator beginRawBuffer() {
		return Iterator(rawBuffer, 0);
	}
	inline const Iterator endRawBuffer() {
		return Iterator(rawBuffer, numPixels);
	}
	
	inline const Iterator beginFxBuffer() {
		return Iterator(fxBuffer, 0);
	}
	inline const Iterator endFxBuffer() {
		return Iterator(fxBuffer, numPixels);
	}
	
	WS28xxStrip(uint16_t numPixels = 0) : numPixels(numPixels) {
	}
	WS28xxStrip(uint16_t numPixels, uint32_t *rawBuffer, uint32_t *fxBuffer) : WS28xxStrip(numPixels) {
		setPixelBuffers(rawBuffer, fxBuffer);
	}
};

template<typename StripType>
struct WS28xxStrip<StripType>::Iterator {
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = uint32_t;
	using pointer = uint32_t*;
	using const_ref = const uint32_t&;

	inline bool operator==(Iterator other) const {
		return this->data == other.data && this->index == other.index;
	}
	inline bool operator!=(Iterator other) const {
		return !(*this == other);
	}
	
	inline Iterator& operator++() {
		index++;
		return *this;
	}
	inline Iterator operator++(int) {
		auto tmp = *this;
		++(*this);
		return tmp;
	}
	
	inline Iterator& operator--() {
		index = (index > 0) ? index - 1 : 0;
		return *this;
	}
	inline Iterator operator--(int) {
		auto tmp = *this;
		--(*this);
		return tmp;
	}
	
	inline const_ref operator*() const {
		return *(data + index);
	}
	
	inline Iterator& operator=(const Iterator &other) {
		if (this != &other) {
			this->data = other.data;
			this->index = other.index;
		}
		
		return *this;
	}
	
	Iterator() = default;
	inline Iterator(pointer _data) : data(_data), index(0) {
	}
	inline Iterator(pointer _data, uint16_t _index) : data(_data), index(_index) {
	}
	inline Iterator(const Iterator &other) : data(other.data), index(other.index) {
	}
	
private:
	pointer data = nullptr;
	uint16_t index = 0;
	
};

using WS2815Strip = WS28xxStrip<WS2815StripType>;

} /* namespace JLED */

#endif /* JLED_INCLUDE_WS28XXSTRIP_HPP_ */
