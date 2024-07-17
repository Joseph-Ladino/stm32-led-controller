/*
 * WS28xxStrip.hpp
 *
 *  Created on: Jul 13, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_WS28XXSTRIP_HPP_
#define JLED_INCLUDE_WS28XXSTRIP_HPP_

#include <cstring>
#include "StripTypes.hpp"

namespace JLED {

struct WSStripState {
	bool powerOn = true;
	uint8_t brightness = 255;
	uint32_t color;
	ColorCorrection colorCorrection = ColorCorrection::Uncorrected;
	TemperatureCorrection tempCorrection = TemperatureCorrection::Uncorrected;
	bool gammaEnabled = true;

	// TODO: add effect and current mode

//	inline bool operator==(const WSStripState &other) {
//		return powerOn == other.powerOn && brightness == other.brightness && color == other.color && colorCorrection == other.colorCorrection && tempCorrection == other.tempCorrection;
//	}

	inline bool operator==(const WSStripState &other) {
//		return num1 == other.num1 && num2 == other.num2;
//		return num == other.num;
		return std::memcmp(this, &other, sizeof(WSStripState)) == 0;
	}
	inline bool operator!=(const WSStripState &other) {
		return !(*this == other);
	}
};

struct StripIterator {
	using StripIterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = uint32_t;
	using pointer = uint32_t*;
	using const_ref = const uint32_t&;

	inline bool operator==(const StripIterator &other) const {
		return this->data == other.data && this->index == other.index;
	}
	inline bool operator!=(const StripIterator &other) const {
		return !(*this == other);
	}
	inline bool operator>(const StripIterator &other) const {
		return this->data == other.data && index > other.index;
	}
	inline bool operator>=(const StripIterator &other) const {
		return this->data == other.data && index >= other.index;
	}
	inline bool operator<(const StripIterator &other) const {
		return this->data == other.data && index < other.index;
	}
	inline bool operator<=(const StripIterator &other) const {
		return this->data == other.data && index <= other.index;
	}

	inline StripIterator& operator++() {
		index++;
		return *this;
	}
	inline StripIterator operator++(int) {
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	inline StripIterator& operator--() {
		index = (index > 0) ? index - 1 : 0;
		return *this;
	}
	inline StripIterator operator--(int) {
		auto tmp = *this;
		--(*this);
		return tmp;
	}

	inline const_ref operator*() const {
		return *(data + index);
	}

	inline StripIterator& operator=(const StripIterator &other) {
		if (this != &other) {
			this->data = other.data;
			this->index = other.index;
		}

		return *this;
	}

	StripIterator() = default;
	inline StripIterator(pointer _data) : data(_data), index(0) {
	}
	inline StripIterator(pointer _data, uint16_t _index) : data(_data), index(_index) {
	}
	inline StripIterator(const StripIterator &other) : data(other.data), index(other.index) {
	}

private:
	pointer data = nullptr;
	uint16_t index = 0;

};

#define ASSERT_STRIPBASE_IS_DERIVED(StripBase, StripType) static_assert(std::is_base_of_v<JLED::WS28xxStrip<StripType>, StripBase>);

/**
 * @brief Basic control class for an LED strip
 * @tparam StripType Strip characteristics (must be derived from JLED::StripType)
 */
template<typename _StripType>
class WS28xxStrip {
public:
	ASSERT_STRIPTYPE_IS_DERIVED(_StripType);
	using PhysicalPowerCB = void(*)(bool);
	using StripType = _StripType;

private:
	const uint16_t numPixels;
	WSStripState state;
	PhysicalPowerCB setPhysicalPower = nullptr;

protected:
	uint32_t *rawBuffer = nullptr, *fxBuffer = nullptr;

public:

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

		state.color = color;
	}

	/**
	 * @brief Set entire strip to color
	 * @param color 0xWWRRGGBB
	 */
	inline void setStaticColor(uint32_t color) {
		setAll(color);
	}

	/**
	 * @brief Retrieve the last set static color
	 * @return color 0xWWRRGGBB
	 */
	inline uint32_t getStaticColor() {
		return state.color;
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
	
	inline WSStripState getStripState() {
		return state;
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

	inline void setColorCorrection(ColorCorrection cc) {
		state.colorCorrection = cc;
	}

	inline ColorCorrection getColorCorrection() {
		return state.colorCorrection;
	}

	inline void setTemperatureCorrection(TemperatureCorrection tc) {
		state.tempCorrection = tc;
	}

	inline TemperatureCorrection getTemperatureCorrection() {
		return state.tempCorrection;
	}

	inline void setGammaCorrection(bool on) {
		state.gammaEnabled = on;
	}

	inline bool getGammaCorrection() {
		return state.gammaEnabled;
	}

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

		if (rawBuffer != nullptr && fxBuffer != nullptr) {
			memset(rawBuffer, 0, sizeof(rawBuffer[0]) * numPixels);
			memset(fxBuffer, 0, sizeof(fxBuffer[0]) * numPixels);
		}
	}
	
	/**
	 * @brief Transfers color data from raw buffer to fx buffer with brightness and effects applied
	 * The process of transferring color data to the physical LED strip is to be defined in a derived class. The derived class should still call this function before transferring data
	 */
	inline void display() {
		if (!state.powerOn) return;
		
		// TODO: filters/effects/animations
		static Color colCrt = state.colorCorrection, tmpCrt = state.tempCorrection;

		uint32_t adjust[4];
		for (int i = 0; i < 4; i++) {
			adjust[i] = ((((uint32_t) colCrt[i]) + 1) * (((uint32_t) tmpCrt[i]) + 1) * state.brightness) >> 16;
		}

		// at the expense of 256 extra bytes of memory, this solution felt cleaner
		const uint8_t *gammaLookupArr = state.gammaEnabled ? gammaCorrectionLookup : gammaRawLookup;

		for (uint16_t i = 0; i < numPixels; i++) {
			Color pixel = rawBuffer[i];
			for (uint8_t j = 0; j < 4; j++) {
				pixel[j] = ((uint32_t) gammaLookupArr[pixel[j]] * adjust[j]) >> 8;
			}

			fxBuffer[i] = StripType::getOrderedBytes(pixel);
		}
	}
	
	inline const StripIterator begin() {
		return beginRawBuffer();
	}
	inline const StripIterator end() {
		return endRawBuffer();
	}
	
	inline const StripIterator beginRawBuffer() {
		return StripIterator(rawBuffer, 0);
	}
	inline const StripIterator endRawBuffer() {
		return StripIterator(rawBuffer, numPixels);
	}
	
	inline const StripIterator beginFxBuffer() {
		return StripIterator(fxBuffer, 0);
	}
	inline const StripIterator endFxBuffer() {
		return StripIterator(fxBuffer, numPixels);
	}
	
	WS28xxStrip(uint16_t numPixels = 0) : numPixels(numPixels) {
	}
	WS28xxStrip(uint16_t numPixels, uint32_t *rawBuffer, uint32_t *fxBuffer) : WS28xxStrip(numPixels) {
		setPixelBuffers(rawBuffer, fxBuffer);
	}
};

using WS2815Strip = WS28xxStrip<WS2815StripType>;

} /* namespace JLED */

#endif /* JLED_INCLUDE_WS28XXSTRIP_HPP_ */
