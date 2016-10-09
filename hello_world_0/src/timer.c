/*
 * timer.c
 *
 *  Created on: Oct 4, 2016
 *      Author: superman
 */

#include <stdio.h>          // xil_printf and so forth.
#include <stdint.h>
#include <stdlib.h>
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "timer.h"
#include "events.h"

// Timing/clock constants - multiply by 10 to get time in milliseconds
#define ONE_SECOND_COUNT 100 // timer ticks in one second
#define BUTTON_POLL_COUNT 5 // poll rate for buttons
#define BULLETS_UPDATE_COUNT 3 // refresh rate for bullets
#define ALIENS_UPDATE_COUNT 70 // refresh rate for aliens
#define MAX_ALIENS_FIRE_COUNT 300 // maximum time between alien bullets
#define ALIEN_EXPLOSION_COUNT 15 // time until an alien explosion sprite gets erased

// appear about every 30 seconds
// scuttle across the screen in 10-15 seconds
#define UFO_UPDATE_COUNT // TODO do this

// Bit masks
#define RIGHT_BUTTON 0x02
#define MIDDLE_BUTTON 0x01
#define LEFT_BUTTON 0x08

// global variables
static uint32_t buttonCounter;
static uint32_t bulletsCounter;
static uint32_t aliensCounter;
static uint32_t aliensFire;
static uint32_t heartbeatCounter;
static uint32_t alienExplosionCounter;

static XGpio gpPB;

// This is invoked in response to a timer interrupt every 10 ms.
void timerInterruptHandler() {
	// Decrement every counter; queue event when a counter reaches zero and
	// reset the counter

	if (--buttonCounter == 0) {
		buttonCounter = BUTTON_POLL_COUNT;
		// Read the buttons and queue the appropriate events
		uint32_t buttons = XGpio_DiscreteRead(&gpPB, 1);
		if (buttons & RIGHT_BUTTON) {
			setEvent(RIGHT_BTN_EVENT);
		}
		if (buttons & MIDDLE_BUTTON) {
			setEvent(MIDDLE_BTN_EVENT);
		}
		if (buttons & LEFT_BUTTON) {
			setEvent(LEFT_BTN_EVENT);
		}
	}

	if (--bulletsCounter == 0) {
		bulletsCounter = BULLETS_UPDATE_COUNT;
		setEvent(BULLETS_REFRESH_EVENT);
	}

	if (--aliensCounter == 0) {
		aliensCounter = ALIENS_UPDATE_COUNT;
		setEvent(ALIENS_REFRESH_EVENT);
	}

	if (--aliensFire == 0) {
		aliensFire = rand() % MAX_ALIENS_FIRE_COUNT + 1;
		setEvent(ALIENS_FIRE_EVENT);
	}

	if(alienExplosionCounter != 0){
		if(--alienExplosionCounter == 0){
			setEvent(ALIEN_DEATH_EVENT);
		}
	}

	if (--heartbeatCounter == 0) {
		heartbeatCounter = ONE_SECOND_COUNT;
		setEvent(HEARTBEAT_EVENT);
	}
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
	int32_t intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK) {
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timerInterruptHandler();
	}
}

void timerInit() {
	// Initialize the GPIO peripherals.
	int32_t success;
	success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);

	// Initialize interrupts - only have the FIT interrupt
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();

	buttonCounter = BUTTON_POLL_COUNT;
	bulletsCounter = BULLETS_UPDATE_COUNT;
	aliensCounter = ALIENS_UPDATE_COUNT;
	aliensFire = rand() % MAX_ALIENS_FIRE_COUNT + 1;
	heartbeatCounter = ONE_SECOND_COUNT;
}

void setAlienExplosionCounter() {
	alienExplosionCounter = ALIEN_EXPLOSION_COUNT;
}
