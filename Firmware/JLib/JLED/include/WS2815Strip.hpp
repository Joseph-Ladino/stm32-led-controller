/*
 * WS2815Strip.hpp
 *
 *  Created on: May 5, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_WS2815STRIP_HPP_
#define JLED_INCLUDE_WS2815STRIP_HPP_

#include <cstdint>
#include <iterator>
#include <string>
#include "Color.hpp"

namespace JLED {

template<uint16_t _NUM_PIXELS = 5>
class WS2815Strip {
public:
	const static uint16_t NUM_PIXELS = _NUM_PIXELS;
	const static uint8_t NUM_BYTES_PER_PIXEL = 3;
	const static uint32_t NUM_BYTES_TOTAL = NUM_PIXELS * NUM_BYTES_PER_PIXEL;

	using physicalPowerCBSig = void(*)(bool);

	/**
	 * @brief Timing in nanoseconds for LED strip data transfer
	 */
	enum TIMING_NS {
		ZERO_L = 650, /**< low time to send a 0 */
		ZERO_H = 350, /**< high time to send a 0 */
		ONE_L = 350, /**< low time to send a 1 */
		ONE_H = 700, /**< high time to send a 1 */
		RES = 300000, /**< RESET time */
		PERIOD = 1250, /**< PWM period */
	};

	uint32_t raw[NUM_PIXELS]; // raw color values
	uint32_t transformed[NUM_PIXELS]; // color values with effects (brightness, power) applied

//	uint32_t *raw, *transformed;

	uint32_t& operator[](uint16_t index);
	const uint32_t& operator[](uint16_t index) const;

	void set(uint16_t index, Color &col);
	Color at(uint16_t index);

	void setAll(Color &col);
	void setAll(uint32_t col);

	void setBrightness(uint8_t brightness);
	uint8_t getBrightness();

	void setPhysicalPowerCB(physicalPowerCBSig cb);
	void setPower(bool powerOn);
	bool getPower();

	virtual void display();

	WS2815Strip();

	struct iterator {
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = uint32_t;
		using pointer = uint32_t*;
		using const_ref = const uint32_t&;

		pointer data = nullptr;
		uint16_t index = 0;

		bool operator==(iterator other) const {
			return this->data == other.data && this->index == other.index;
		}
		bool operator!=(iterator other) const {
			return !(*this == other);
		}

		iterator& operator++() {
			index++;
			return *this;
		}

		iterator operator++(int) {
			auto tmp = *this;
			++(*this);
			return tmp;
		}

		iterator& operator--() {
			index = (index > 0) ? index - 1 : 0;
			return *this;
		}

		iterator operator--(int) {
			auto tmp = *this;
			--(*this);
			return tmp;
		}

		const_ref operator*() const {
			return *(data + index);
		}

		iterator& operator=(const iterator &other) {
			if (this != &other) {
				this->data = other.data;
				this->index = other.index;
			}

			return *this;
		}

		iterator() = default;
		iterator(pointer _data) :
				data(_data), index(0) {
		}
		iterator(pointer _data, uint16_t _index) :
				data(_data), index(_index) {
		}

		iterator(const iterator &other) :
				data(other.data), index(other.index) {
		}
	};

	inline const iterator begin() {
		return iterator(raw);
	}
	inline const iterator end() {
		return iterator(raw, NUM_PIXELS); // note: end() is not in bounds
	}

	inline const iterator beginT() {
		return iterator(transformed);
	}
	inline const iterator endT() {
		return iterator(transformed, NUM_PIXELS);
	}

private:
	uint8_t brightness = 255;
	bool powerOn = true;

	physicalPowerCBSig physicalPowerCB = nullptr;
};

template<uint16_t NUM_PIXELS>
inline uint32_t& WS2815Strip<NUM_PIXELS>::operator [](uint16_t index) {
	return raw[index];
}

template<uint16_t NUM_PIXELS>
inline const uint32_t& WS2815Strip<NUM_PIXELS>::operator [](
		uint16_t index) const {
	return raw[index];
}

template<uint16_t NUM_PIXELS>
inline void WS2815Strip<NUM_PIXELS>::set(uint16_t index, Color &col) {
	raw[index] = col.num;
}

template<uint16_t NUM_PIXELS>
inline Color WS2815Strip<NUM_PIXELS>::at(uint16_t index) {
	return Color(raw[index]);
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::setAll(Color &col) {
	setAll(col.num);
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::setAll(uint32_t col) {
	for (auto i = 0; i < NUM_PIXELS; i++) {
		raw[i] = col;
	}
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::display() {
//	memcpy(transformed, raw, sizeof(raw[0])* NUM_PIXELS);
	if(!powerOn) return;

	for(uint16_t i = 0; i < NUM_PIXELS; i++) {
		transformed[i] = raw[i] * brightness / 255;
	}
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::setBrightness(uint8_t brightness) {
	this->brightness = brightness;
}

template<uint16_t _NUM_PIXELS>
inline uint8_t WS2815Strip<_NUM_PIXELS>::getBrightness() {
	return brightness;
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::setPower(bool powerOn) {
	this->powerOn = powerOn;

	if(physicalPowerCB != nullptr) {
		physicalPowerCB(powerOn);
	}

	if(!powerOn) {
		memset(transformed, 0, sizeof(transformed[0]) * NUM_PIXELS);
	}
}

template<uint16_t _NUM_PIXELS>
inline bool WS2815Strip<_NUM_PIXELS>::getPower() {
	return powerOn;
}

template<uint16_t _NUM_PIXELS>
inline void WS2815Strip<_NUM_PIXELS>::setPhysicalPowerCB(physicalPowerCBSig cb) {
	physicalPowerCB = cb;
}

template<uint16_t NUM_PIXELS>
inline WS2815Strip<NUM_PIXELS>::WS2815Strip() {
//	memset(raw, 0, NUM_BYTES_TOTAL);
//	raw = new uint32_t[NUM_PIXELS];
//	transformed = new uint32_t[NUM_PIXELS];

	memset(raw, 0, NUM_PIXELS * sizeof(raw[0]));
}

} /* namespace JLED */

#endif /* JLED_INCLUDE_WS2815STRIP_HPP_ */
