/*
 * DMAStrip2.hpp
 *
 *  Created on: Jul 13, 2024
 *      Author: user
 */

#ifndef JLED_INCLUDE_DMASTRIP2_HPP_
#define JLED_INCLUDE_DMASTRIP2_HPP_

#include "WS28xxStrip.hpp"

namespace JLED {

template<typename StripType>
class DMAStrip2: public WS28xxStrip<StripType> {
	ASSERT_STRIPTYPE_IS_DERIVED(StripType);

public:
	using LEDBase = WS28xxStrip<StripType>;
	using DMABufferType = uint8_t;
	//	using DMA_START_CB = std::function<void(DMA_BUF_TYPE*, uint16_t)>;
	//	using DMA_STOP_CB = std::function<void()>;
	using DMA_START_CB = void (*)(DMABufferType*, uint16_t);
	using DMA_STOP_CB = void(*)(void);

	enum class DMAStatus : uint8_t {
		IN_PROGRESS, RESET_STARTED, RESET_IN_PROGRESS, RESET_COMPLETE, DONE,
	};

private:
	const uint16_t numPixelsPerDMA, dmaBufferSize;
	uint8_t dmaLowBit = 0, dmaHighBit = 0;
	uint16_t dmaARR = 0;
	DMABufferType *dmaBuffer = nullptr;
	DMA_START_CB dmaStart = nullptr;
	DMA_STOP_CB dmaStop = nullptr;

	typename LEDBase::Iterator dmaLedIT, dmaLedEndIT;

	DMAStatus dmaTransferStatus = DMAStatus::DONE;
public:

	inline static constexpr uint32_t calcDMABufferSize(uint16_t numPixelsPerDMA) {
		return 2 * numPixelsPerDMA * StripType::bitsPerPixel;
	}
	
	inline void colorToDMATiming(uint32_t col, DMABufferType **outPtr) {
		static constexpr uint8_t bitOffset = 32 - StripType::bitsPerPixel;
		col = col << bitOffset;
		
		for (uint8_t b = 0; b < StripType::bitsPerPixel; b++) {
			(*outPtr)[b] = ((col << b) & 0x80000000) == 0 ? dmaLowBit : dmaHighBit;
		}
		(*outPtr) += StripType::bitsPerPixel;
	}
	
	inline void setStartDMACallback(DMA_START_CB dmaStart) {
		this->dmaStart = dmaStart;
	}
	inline void setStopDMACallback(DMA_STOP_CB dmaStart) {
		this->dmaStop = dmaStart;
	}
	
	inline void setARR(uint16_t dmaARR) {
		this->dmaARR = dmaARR;
		
		dmaLowBit = dmaARR * StripType::zeroHighNs / StripType::pwmPeriodNs;
		
		dmaHighBit = dmaARR * StripType::oneHighNs / StripType::pwmPeriodNs;
	}
	
	inline void setDMABuffer(DMABufferType *dmaBuffer) {
		this->dmaBuffer = dmaBuffer;
	}
	
	inline void display() {
		if (displayInProgress()) return;
		
		LEDBase::display(); // copy raw buffer to transformed buffer
		
		dmaLedIT = LEDBase::beginFxBuffer();
		dmaTransferStatus = DMAStatus::IN_PROGRESS;
		dmaStart(dmaBuffer, dmaBufferSize);
	}
	
	inline bool displayInProgress() {
		return dmaTransferStatus != DMAStatus::DONE;
	}
	
	inline void onDMAInterrupt(bool halfCompleteInterrupt) {
		
		static DMABufferType *leftHalf = dmaBuffer, *rightHalf = dmaBuffer + dmaBufferSize / 2;
		
		// buffer has been completely set to reset value, dma can be stopped
		if (dmaTransferStatus == DMAStatus::RESET_COMPLETE) {
			dmaTransferStatus = DMAStatus::DONE;
			dmaStop();
			
			//			frameDelay.reset();
			
			return;
		}
		
		DMABufferType *writeBuf = halfCompleteInterrupt ? leftHalf : rightHalf;
		
		int curPixel;
		for (curPixel = 0; curPixel < numPixelsPerDMA && dmaLedIT != dmaLedEndIT; curPixel++, dmaLedIT++) {
			auto led = *dmaLedIT;
			colorToDMATiming(led, &writeBuf);
		}
		
		// all data in ledstrip has been written but the DMA buffer is not full of reset data
		if (curPixel != numPixelsPerDMA) {
			auto fillPixels = numPixelsPerDMA - curPixel;
			
			// fill with logic low value to transition to WS28xx reset period
			memset(writeBuf, 0, (uint32_t) fillPixels * StripType::bitsPerPixel * sizeof(DMABufferType));
			
			if (fillPixels == numPixelsPerDMA) {
				dmaTransferStatus =
						(dmaTransferStatus == DMAStatus::RESET_STARTED) ?
								DMAStatus::RESET_IN_PROGRESS : DMAStatus::RESET_COMPLETE;
			} else {
				dmaTransferStatus = DMAStatus::RESET_STARTED;
			}
		}
	}
	
	inline DMAStrip2(uint16_t numPixels = 0, uint16_t numPixelsPerDMA = 0) : LEDBase(numPixels), numPixelsPerDMA(
			numPixelsPerDMA), dmaBufferSize(calcDMABufferSize(numPixelsPerDMA)) {
		dmaLedEndIT = LEDBase::endFxBuffer();
	}
	
	inline DMAStrip2(uint16_t numPixels, uint16_t numPixelsPerDMA, uint32_t *rawBuffer, uint32_t *fxBuffer,
			DMABufferType *dmaBuffer) : LEDBase(numPixels, rawBuffer, fxBuffer), numPixelsPerDMA(numPixelsPerDMA), dmaBufferSize(
			calcDMABufferSize(numPixelsPerDMA)) {
		setDMABuffer(dmaBuffer);
	}
}
;

} /* namespace JLED */

#endif /* JLED_INCLUDE_DMASTRIP2_HPP_ */