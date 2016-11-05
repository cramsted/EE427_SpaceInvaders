/*
 * pit.h
 *
 *  Created on: Nov 3, 2016
 *      Author: superman
 */

#ifndef PIT_H_
#define PIT_H_
#include <stdint.h>

#define XPAR_PIT_0_BASEADDR 0x7bc00000
#define PIT_INITIAL_DELAY 999999

void pitInit(uint32_t delay);
void pitEnableInterrupt();
void pitDisableInterrupt();
void pitSetDelay(uint32_t delay);
void pitStart();
void pitStop();
void pitReloadEnable();
void pitReloadDisable();


#endif /* PIT_H_ */
