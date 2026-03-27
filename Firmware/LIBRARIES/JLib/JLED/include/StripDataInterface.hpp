/*
 * WS28xxInterface.hpp
 *
 *  Created on: Jul 30, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_WS28XXINTERFACE_HPP_
#define JLED_INCLUDE_WS28XXINTERFACE_HPP_

#include <cstdint>

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

struct StripDataInterface {
	virtual uint32_t& operator[](uint16_t index) = 0;
	virtual const uint32_t& operator[](uint16_t index) const = 0;
	virtual void setAll(uint32_t color) = 0;
	virtual uint16_t getNumPixels() const = 0;
	virtual const StripIterator begin() = 0;
	virtual const StripIterator end() = 0;
};


#endif /* JLED_INCLUDE_WS28XXINTERFACE_HPP_ */
