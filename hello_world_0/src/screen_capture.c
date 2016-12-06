/*
 * screen_capture.c
 *
 *  Created on: Dec 6, 2016
 *      Author: superman
 */
#include <stdio.h>          // xil_printf and so forth.
#include <stdint.h>
#include <stdlib.h>
#include "screen_capture.h"
#include "xparameters.h"
#include "xgpio.h"          // Provides access to PB GPIO driver.

// Switch bit masks
#define SW_7 0x80
#define SW_6 0x40
#define SW_5 0x20

XGpio gpSW;



void readSwitches(){
	xil_printf("read switches");
	uint32_t switches = XGpio_DiscreteRead(&gpSW, 1);
	xil_printf("switch vals: %x\r\n", switches);

	// Read the switches and queue the appropriate events
//		uint32_t buttons = XGpio_DiscreteRead(&gpPB, 1);
//		if (buttons & RIGHT_BUTTON) {
//			setEvent(RIGHT_BTN_EVENT);
//		}
//		if (buttons & MIDDLE_BUTTON) {
//			setEvent(MIDDLE_BTN_EVENT);
//		}
//		if (buttons & LEFT_BUTTON) {
//			setEvent(LEFT_BTN_EVENT);
//		}
//		if (buttons & TOP_BUTTON) {
//			increaseVolume();
//		}
//		if (buttons & BOTTOM_BUTTON) {
//			decreaseVolume();
//		}
}

// initializes the switches hardware for the screen capture
void initScreenCapture(){
	xil_printf("switches init");
	 // Initialize the GPIO peripherals.
	int32_t switches;
	switches = XGpio_Initialize(&gpSW, XPAR_SWITCHES_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpSW, 1, 0x0000001F);
}



