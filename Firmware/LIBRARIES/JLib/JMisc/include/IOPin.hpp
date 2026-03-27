/*
 * PinState.hpp
 *
 *  Created on: Nov 7, 2024
 *      Author: user
 */

#ifndef JMISC_INCLUDE_IOPIN_HPP_
#define JMISC_INCLUDE_IOPIN_HPP_

struct IOPin {
protected:
	bool pinSet = false;
	inline virtual void setPhysicalState(bool powerOn) {
		// OVERRIDE IN CHILD CLASS
	}
	
	inline virtual bool getPhysicalState() {
		// OVERRIDE IN CHILD CLASS
		return pinSet;
	}
	
public:
	inline void set(bool on) {
			this->pinSet = on;
			
			setPhysicalState(on);
		}
	
	inline bool get() {
		return pinSet && getPhysicalState();
	}
	
	inline IOPin(bool on) {
		set(on);
	}
};

#endif /* JMISC_INCLUDE_IOPIN_HPP_ */
