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

void pitInit(uint32_t delay) {
	pitSetDelay(delay);
	pitReloadEnable();
	pitEnableInterrupt();
}

void pitEnableInterrupt() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_INTERRUPT_BIT);
}

void pitDisableInterrupt() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	controlReg &= ~CTRL_INTERRUPT_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}

void pitSetDelay(uint32_t delay) {
	if (delay < PIT_MINIMUM_DELAY) {
		delay = PIT_MINIMUM_DELAY;
	}
	Xil_Out32(DELAY_REG_ADDR, delay);
}

void pitStart() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_RUN_BIT);
}

void pitStop() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	controlReg &= ~CTRL_RUN_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}

void pitReloadEnable() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	Xil_Out32(CTRL_REG_ADDR, controlReg | CTRL_RELOAD_BIT);
}

void pitReloadDisable() {
	uint32_t controlReg = Xil_In32(CTRL_REG_ADDR);
	controlReg &= ~CTRL_RELOAD_BIT;
	Xil_Out32(CTRL_REG_ADDR, controlReg);
}
