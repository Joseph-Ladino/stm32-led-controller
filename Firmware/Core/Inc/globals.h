/*
 * globals.h
 *
 *  Created on: Feb 17, 2024
 *      Author: user
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdio.h"

#define ETH_DEBUG


//void USB_Printf(const char* format, ...);
#define USB_Printf(...) printf(__VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /* INC_GLOBALS_H_ */
