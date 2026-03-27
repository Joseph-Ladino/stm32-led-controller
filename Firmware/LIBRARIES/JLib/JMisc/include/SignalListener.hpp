/*
 * SignalListener.hpp
 *
 *  Created on: Jul 9, 2024
 *      Author: user
 */

#ifndef JMISC_INCLUDE_SIGNALLISTENER_HPP_
#define JMISC_INCLUDE_SIGNALLISTENER_HPP_

template<typename RETURN_T = void, typename ARG_T = void>
class SignalListener {
public:
	virtual RETURN_T signal(ARG_T) = 0;
};

#endif /* JMISC_INCLUDE_SIGNALLISTENER_HPP_ */
