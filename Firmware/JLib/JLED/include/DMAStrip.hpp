/*
 * DMAStrip.hpp
 *
 *  Created on: Jul 7, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_DMASTRIP_HPP_
#define JLED_INCLUDE_DMASTRIP_HPP_

#include <functional>
#include <cmath>
#include "WS2815Strip.hpp"
#include "CountdownTimer.hpp"

namespace JLED {
template<uint16_t _NUM_PIXELS = 10, uint16_t _NUM_PIXELS_PER_DMA = 5,
		uint16_t _DMA_ARR = 79, typename DMA_BUF_TYPE = uint32_t>
class DMAStrip: public WS2815Strip<_NUM_PIXELS> {

public:
	using LEDBase = WS2815Strip<_NUM_PIXELS>;
	const static auto NUM_PIXELS = LEDBase::NUM_PIXELS;

	const static auto NUM_PIXELS_PER_DMA = _NUM_PIXELS_PER_DMA;
	const static auto HALF_DMA_BUF_LEN = NUM_PIXELS_PER_DMA
			* LEDBase::NUM_BYTES_PER_PIXEL * 8;
	const static auto FULL_DMA_BUF_LEN = HALF_DMA_BUF_LEN * 2;
	const static auto NUM_ELEMENTS_PER_RESET = LEDBase::RES / LEDBase::PERIOD;
	const static auto DMA_ARR = _DMA_ARR;
	const static uint16_t DMA_BIT_ONE = (float) LEDBase::ONE_H / LEDBase::PERIOD
			* DMA_ARR;
	const static uint16_t DMA_BIT_ZERO = (float) LEDBase::ZERO_H
			/ LEDBase::PERIOD * DMA_ARR;

	enum DMAStatus {
		IN_PROGRESS, RESET_STARTED, RESET_IN_PROGRESS, RESET_COMPLETE, DONE,
	};

//	DMA_BUF_TYPE dmaBuffer[FULL_DMA_BUF_LEN];
	DMA_BUF_TYPE* dmaBuffer;
	typename LEDBase::iterator dmaLedIT, dmaLedEndIT;
	DMAStatus dmaTransferStatus = DONE;

	CountdownTimer frameDelay {1};

	using DMA_START_CB = std::function<void(DMA_BUF_TYPE*, uint16_t)>;
	using DMA_STOP_CB = std::function<void()>;
	DMA_START_CB dmaStart;
	DMA_STOP_CB dmaStop;
	void setStartDMACallback(DMA_START_CB cb);
	void setStopDMACallback(DMA_STOP_CB cb);

	void display();
	void byteToDMATiming(uint8_t byte, DMA_BUF_TYPE **outPtr);
	void onDMAInterrupt(bool halfCompleteInterrupt);
	bool displayInProgress();

	DMAStrip();
};

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline void DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::setStartDMACallback(
		DMA_START_CB cb) {
	dmaStart = cb;
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline void DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::setStopDMACallback(
		DMA_STOP_CB cb) {
	dmaStop = cb;
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline void DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::display() {

	if (displayInProgress())
		return;

	LEDBase::display(); // copy raw buffer to transformed buffer

	dmaLedIT = LEDBase::beginT();
	dmaTransferStatus = IN_PROGRESS;
	dmaStart(dmaBuffer, FULL_DMA_BUF_LEN);
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline void DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::byteToDMATiming(
		uint8_t byte, DMA_BUF_TYPE **outPtr) {
	for (uint8_t b = 0; b < 8; b++) {
		(*outPtr)[b] = ((byte << b) & 0x80) == 0 ? DMA_BIT_ZERO : DMA_BIT_ONE;
	}
	(*outPtr) += 8;
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline void DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::onDMAInterrupt(
		bool halfCompleteInterrupt) {

	// buffer has been completely set to reset value, dma can be stopped
	if (dmaTransferStatus == RESET_COMPLETE) {
		dmaTransferStatus = DONE;
		dmaStop();

		frameDelay.reset();

		return;
	}

	DMA_BUF_TYPE *writeBuf =
			halfCompleteInterrupt ?
					(DMA_BUF_TYPE*) dmaBuffer :
					(DMA_BUF_TYPE*) (dmaBuffer + HALF_DMA_BUF_LEN);

	int curLed;
	for (curLed = 0; curLed < NUM_PIXELS_PER_DMA && dmaLedIT != dmaLedEndIT;
			curLed++, dmaLedIT++) {
		Color led = *dmaLedIT;
		byteToDMATiming(led.g, &writeBuf);
		byteToDMATiming(led.r, &writeBuf);
		byteToDMATiming(led.b, &writeBuf);
	}

	// all data in ledstrip has been written but the DMA buffer is not full of reset data
	if (curLed != NUM_PIXELS_PER_DMA) {
		auto fillLeds = NUM_PIXELS_PER_DMA - curLed;

		// fill with logic low value to transition to WS2815 reset period
		memset(writeBuf, 0,
				fillLeds * LEDBase::NUM_BYTES_PER_PIXEL * 8
						* sizeof(DMA_BUF_TYPE));

		if(fillLeds == NUM_PIXELS_PER_DMA) {
			dmaTransferStatus = (dmaTransferStatus == RESET_STARTED) ? RESET_IN_PROGRESS : RESET_COMPLETE;
		} else {
			dmaTransferStatus = RESET_STARTED;
		}
//		if(fillLeds == NUM_PIXELS_PER_DMA && i++ >= 2) dmaTransferStatus = RESET_COMPLETE;
//		else dmaTransferStatus = RESET_STARTED;

//		dmaTransferStatus =
//				(fillLeds == NUM_PIXELS_PER_DMA
//						&& dmaTransferStatus == PARTIAL_RESET) ?
//						FULL_RESET : PARTIAL_RESET;
	}
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
inline bool DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::displayInProgress() {
	return dmaTransferStatus != DONE || !frameDelay.expired();
}

template<uint16_t _NUM_PIXELS, uint16_t _NUM_PIXELS_PER_DMA, uint16_t _DMA_ARR,
		typename DMA_BUF_TYPE>
DMAStrip<_NUM_PIXELS, _NUM_PIXELS_PER_DMA, _DMA_ARR, DMA_BUF_TYPE>::DMAStrip() : LEDBase::WS2815Strip() {
	dmaBuffer = new DMA_BUF_TYPE[FULL_DMA_BUF_LEN];

	memset((DMA_BUF_TYPE*) dmaBuffer, 0,
			FULL_DMA_BUF_LEN * sizeof(DMA_BUF_TYPE));
	dmaLedIT = LEDBase::beginT();
	dmaLedEndIT = LEDBase::endT();
}

}

#endif /* JLED_INCLUDE_DMASTRIP_HPP_ */
