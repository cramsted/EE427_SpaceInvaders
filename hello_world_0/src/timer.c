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
#include "tank.h"
#include "events.h"
#include "audio_files/audio.h" // For access to our API for plaing audio
#include "xac97_l.h"
#include "pit.h"

// Timing/clock constants - multiply by 10 to get time in milliseconds
#define ONE_SECOND_COUNT 100 // timer ticks in one second
#define BUTTON_POLL_COUNT 4 // poll rate for buttons
#define UART_POLL_COUNT 4 // poll rate for buttons
#define BULLETS_UPDATE_COUNT 3 // refresh rate for bullets
#define ALIENS_UPDATE_COUNT 70 // refresh rate for aliens
#define MAX_ALIENS_FIRE_COUNT 300 // maximum time between alien bullets
#define ALIEN_EXPLOSION_COUNT 15 // time until an alien explosion sprite gets erased
#define TANK_DEATH_COUNT 150

// UFO appears about every 25-30 seconds
#define UFO_APPEAR_COUNT_MINIMUM 1500
#define UFO_APPEAR_COUNT_MAXIMUM 2000

// scuttle across the screen in 10-15 seconds
#define UFO_UPDATE_COUNT 6
#define UFO_EXPLOSION_COUNT 50

// Bit masks
#define RIGHT_BUTTON 0x02
#define MIDDLE_BUTTON 0x01
#define LEFT_BUTTON 0x08
#define TOP_BUTTON 0x10
#define BOTTOM_BUTTON 0x04

// initializes the following counters
void resetCounters();

// global variables - these counters are updated in the FIT ISR handler
// to keep track of various timing that needs to happen throughout the game
// The variable names explain the function.
static uint32_t buttonCounter;
static uint32_t bulletsCounter;
static uint32_t aliensCounter;
static uint32_t aliensFireCounter;
static uint32_t heartbeatCounter;
static uint32_t alienExplosionCounter;
static uint32_t ufoUpdateCounter;
static uint32_t ufoAppearCounter;
static uint32_t ufoExplosionCounter;
static uint32_t tankDeathCounter;
static uint32_t uartCounter;

// A handle for reading the state of the push buttons
static XGpio gpPB;

// The following update<counterName> functions all do similar work:
// they decrement a counter; when the counter expires,
// they set the appropriate event and reset the counter
// They are called by the FIT ISR handler.
// They are what makes the game engine (events loop) run.

// Decrement the button counter
// When it expires, reset the counter, read the buttons,
// and set button events for whichever buttons were pressed
// The buttons make the tank move and make the tank fire
void updateButtonCounter() {
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
        if (buttons & TOP_BUTTON) {
            increaseVolume();
        }
        if (buttons & BOTTOM_BUTTON) {
            decreaseVolume();
        }
    }
}

// Sets the update bullets event to make the bullets move
void updateBulletsCounter() {
    if (--bulletsCounter == 0) {
        bulletsCounter = BULLETS_UPDATE_COUNT;
        setEvent(BULLETS_REFRESH_EVENT);
    }
}

// The aliens refresh event makes the aliens move
void updateAliensCounter() {
    if (--aliensCounter == 0) {
        aliensCounter = ALIENS_UPDATE_COUNT;
        setEvent(ALIENS_REFRESH_EVENT);
    }
}

// The aliens fire event makes an alien fire a missile
// We use the rand function to make the timing between shots
// pseudo-random
void updateAliensFireCounter() {
    if (--aliensFireCounter == 0) {
        aliensFireCounter = rand() % MAX_ALIENS_FIRE_COUNT + 1;
        setEvent(ALIENS_FIRE_EVENT);
    }
}

// The alien death event erases an exploded alien sprite
void updateAlienExplosionCounter() {
    if (alienExplosionCounter != 0) {
        if (--alienExplosionCounter == 0) {
            setEvent(ALIEN_DEATH_EVENT);
        }
    }
}

// The UFO explosion event erases the points text that appears when the UFO dies
void updateUfoUpdateCounter() {
    if (--ufoUpdateCounter == 0) {
        ufoUpdateCounter = UFO_UPDATE_COUNT;
        setEvent(UFO_UPDATE_EVENT);
    }
}

// The UFO appear event makes the UFO appear on the screen
void updateUfoExplosionCounter() {
    if (ufoExplosionCounter != 0) {
        if (--ufoExplosionCounter == 0) {
            setEvent(UFO_EXPLOSION_EVENT);
        }
    }
}

// The heartbeat event is used for utilization
void updateUfoAppearanceCounter() {
    if (--ufoAppearCounter == 0) {
        resetUfoAppearanceCounter();
        setEvent(UFO_APPEAR_EVENT);
    }
}

// The tank death counter makes the game wait in a paused
// state before re-enabling events and redrawing the tank
// so the game will resume.
void updateHeartbeatCounter() {
    if (--heartbeatCounter == 0) {
        heartbeatCounter = ONE_SECOND_COUNT;
        setEvent(HEARTBEAT_EVENT);
    }
}

// The tank death counter makes the game wait in a paused
// state before re-enabling events and redrawing the tank
// so the game will resume.
void updateTankDeathCounter() {
    playAudio(); // This causes the next frame of the tank death audio to be written to the AC97.
    if (--tankDeathCounter == 0) {
        enableEvents();
        tankDeathCounter = TANK_DEATH_COUNT;
        if (tank.lives != 0) {
            drawTank(tank.p.x, &tank);
        }
    }
}

void updateUartPollCounter(){
    if (--uartCounter == 0) {
    	uartCounter = UART_POLL_COUNT;
        setEvent(UART_EVENT);
    }
}
// This is invoked in response to a timer interrupt every 10 ms.
void timerInterruptHandler() {
	// Decrement every counter; queue event when a counter reaches zero and
	// reset the counter.
    // Most of these counters should only update if events are enabled.
	if (eventsEnabled()) {
		updateButtonCounter();
		updateBulletsCounter();
		updateAliensCounter();
		updateAliensFireCounter();
		updateAlienExplosionCounter();
		updateUfoUpdateCounter();
		updateUfoExplosionCounter();
		updateUfoAppearanceCounter();
		updateUartPollCounter();
	} else {
        // This counter is updated while events are disabled - it will re-enable events when it expires.
		updateTankDeathCounter();
	}
    // The heartbeat counter will always update - used for utilization
	updateHeartbeatCounter();
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
    int32_t intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
    // Check the FIT interrupt first.
    if (intc_status & XPAR_PIT_0_PIT_INTR_MASK) {
        XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_PIT_INTR_MASK);
        timerInterruptHandler();
    }
    // Check the AC97 (sound chip) interrupt. Queue up the audio event if it did interrupt so that
    // another frame of audio will be written to the AC97.
    if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK) {
        XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
        setEvent(AUDIO_EVENT);
    }
}

// Resets all counters that are updated by the FIT ISR handler
void resetCounters() {
    buttonCounter = BUTTON_POLL_COUNT;
    bulletsCounter = BULLETS_UPDATE_COUNT;
    aliensCounter = ALIENS_UPDATE_COUNT;
    aliensFireCounter = rand() % MAX_ALIENS_FIRE_COUNT + 1;
    heartbeatCounter = ONE_SECOND_COUNT;
    tankDeathCounter = TANK_DEATH_COUNT;
    ufoUpdateCounter = UFO_UPDATE_COUNT;
    uartCounter  = UART_POLL_COUNT;
    resetUfoAppearanceCounter();
}

void timerInit() {
    // Initialize the GPIO peripherals.
    int32_t success;
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);

    // Initialize interrupts - only have the FIT interrupt
    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT_0_PIT_INTR_MASK | XPAR_AXI_AC97_0_INTERRUPT_MASK);
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();
	pitInit(PIT_INITIAL_DELAY);
	pitStart();
    // Need to initialize counters for anything to happen
    resetCounters();
}

void setAlienExplosionCounter() {
    // In order to make sure the alien gets erased before the rest of the aliens move,
    // make this counter either its regular value or the counter for moving the aliens - 1,
    // whichever is less
    alienExplosionCounter
            = ALIEN_EXPLOSION_COUNT < (aliensCounter - 1) ? ALIEN_EXPLOSION_COUNT
                    : aliensCounter - 1;
}

// See header file
void setUfoExplosionCounter() {
    ufoExplosionCounter = UFO_EXPLOSION_COUNT;
}

// See header file
// We use rand and a minimum value to make the UFO appear somewhat randomly,
// within a certain amount of time
void resetUfoAppearanceCounter() {
    int32_t temp = (UFO_APPEAR_COUNT_MAXIMUM - UFO_APPEAR_COUNT_MINIMUM);
    ufoAppearCounter = UFO_APPEAR_COUNT_MINIMUM + (rand() % temp);
}
