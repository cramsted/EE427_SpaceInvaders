/*
 * pit.h
 *
 *  Created on: Nov 3, 2016
 *      Author: superman
 */

#ifndef PIT_H_
#define PIT_H_
#include <stdint.h>

#define XPAR_PIT_0_BASEADDR 0x7bc00000	//base address of the PIT timer
#define PIT_INITIAL_DELAY 999999		//default initial delay of the PIT timer

//initializes the PIT with the passed in delay value and the control register zeroed out
void pitInit(uint32_t delay);

//enables interrupts
void pitEnableInterrupt();

//disables interrupts
void pitDisableInterrupt();

//sets the passed in value to the delay register
void pitSetDelay(uint32_t delay);

//starts the PIT timer counting down
void pitStart();

//stops the PIT timer from counting down
void pitStop();

//when the counter reaches zero, this allows the counter to reload the value in the delay
//register and begin counting down again.
void pitReloadEnable();

//when the counter reaches zero, it stays at zero. 
void pitReloadDisable();


#endif /* PIT_H_ */
