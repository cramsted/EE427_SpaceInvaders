/*
 * pit.c
 *
 *  Created on: Nov 3, 2016
 *      Author: superman
 */
#include "pit.h"
#include "xparameters.h"
#include "xil_io.h"

// So the PIT doesn't interrupt too often.
#define PIT_MINIMUM_DELAY 50000

#define CTRL_RUN_BIT 0x01
#define CTRL_INTERRUPT_BIT 0x02
#define CTRL_RELOAD_BIT 0x04

#define CTRL_REG_OFFSET 4
#define DELAY_REG_OFFSET 0

#define CTRL_REG_ADDR (XPAR_PIT_0_BASEADDR + CTRL_REG_OFFSET)
#define DELAY_REG_ADDR XPAR_PIT_0_BASEADDR

//initializes the PIT with the passed in delay value, reloading and interrupts enabled
//@param delay  -  value that can be reloaded into the counter when it reaches zero
void pitInit(uint32_t delay) {
	pitSetDelay(delay);
	pitReloadEnable();
	pitEnableInterrupt();
}

//enables interrupts
void pitEnableInterrupt() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//enables the interrupt bit
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_INTERRUPT_BIT);
}

//disables interrupts
void pitDisableInterrupt() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//disables the interrupt bit
	controlReg &= ~CTRL_INTERRUPT_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}

//sets the passed in value to the delay register
void pitSetDelay(uint32_t delay) {
	//checks if delay value is less than the smallest delay value
	//Note: this is necessiary to keep interrupts from firing so often that the API can't run
	if (delay < PIT_MINIMUM_DELAY) {
		delay = PIT_MINIMUM_DELAY;
	}
	Xil_Out32(DELAY_REG_ADDR, delay);
}

//starts the PIT timer counting down
void pitStart() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//sets run bit
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_RUN_BIT);
}

void pitStop() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//disables the run bit
	controlReg &= ~CTRL_RUN_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}

//when the counter reaches zero, this allows the counter to reload the value in the delay
//register and begin counting down again.
void pitReloadEnable() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//sets the reload bit
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_RELOAD_BIT);
}

void pitReloadDisable() {
	//gets the current value of the control register
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	//disables the reload bit
	controlReg &= ~CTRL_RELOAD_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}
