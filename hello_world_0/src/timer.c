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
#include "audio_files/audio.h"
#include "xac97_l.h"

// Timing/clock constants - multiply by 10 to get time in milliseconds
#define ONE_SECOND_COUNT 100 // timer ticks in one second
#define BUTTON_POLL_COUNT 4 // poll rate for buttons
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

// initializes the following counters
void resetCounters();

// global variables
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

static XGpio gpPB;

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
    }
}

void updateBulletsCounter() {
    if (--bulletsCounter == 0) {
        bulletsCounter = BULLETS_UPDATE_COUNT;
        setEvent(BULLETS_REFRESH_EVENT);
    }
}

void updateAliensCounter() {
    if (--aliensCounter == 0) {
        aliensCounter = ALIENS_UPDATE_COUNT;
        setEvent(ALIENS_REFRESH_EVENT);
    }
}

void updateAliensFireCounter() {
    if (--aliensFireCounter == 0) {
        aliensFireCounter = rand() % MAX_ALIENS_FIRE_COUNT + 1;
        setEvent(ALIENS_FIRE_EVENT);
    }
}

void updateAlienExplosionCounter() {
    if (alienExplosionCounter != 0) {
        if (--alienExplosionCounter == 0) {
            setEvent(ALIEN_DEATH_EVENT);
        }
    }
}

void updateUfoUpdateCounter() {
    if (--ufoUpdateCounter == 0) {
        ufoUpdateCounter = UFO_UPDATE_COUNT;
        setEvent(UFO_UPDATE_EVENT);
    }
}

void updateUfoExplosionCounter() {
    if (ufoExplosionCounter != 0) {
        if (--ufoExplosionCounter == 0) {
            setEvent(UFO_EXPLOSION_EVENT);
        }
    }
}

void updateUfoAppearanceCounter() {
    if (--ufoAppearCounter == 0) {
        resetUfoAppearanceCounter();
        setEvent(UFO_APPEAR_EVENT);
    }
}

void updateHeartbeatCounter() {
    if (--heartbeatCounter == 0) {
        heartbeatCounter = ONE_SECOND_COUNT;
        setEvent(HEARTBEAT_EVENT);
    }
}

// TODO: I need to fix the tank explosion animation so it's not in a while loop
void updateTankDeathCounter() {
    playAudio();
    if (--tankDeathCounter == 0) {
        enableEvents();
        tankDeathCounter = TANK_DEATH_COUNT;
        if (tank.lives != 0) {
            drawTank(tank.p.x, &tank);
        }
    }
}

// This is invoked in response to a timer interrupt every 10 ms.
void timerInterruptHandler() {
    // Decrement every counter; queue event when a counter reaches zero and
    // reset the counter
    if (eventsEnabled()) {
        updateButtonCounter();
        updateBulletsCounter();
        updateAliensCounter();
        updateAliensFireCounter();
        updateAlienExplosionCounter();
        updateUfoUpdateCounter();
        updateUfoExplosionCounter();
        updateUfoAppearanceCounter();
    } else {
        updateTankDeathCounter();
    }
    updateHeartbeatCounter();
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
    if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK) {
        XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
        setEvent(AUDIO_EVENT);
    }
}

void resetCounters() {
    buttonCounter = BUTTON_POLL_COUNT;
    bulletsCounter = BULLETS_UPDATE_COUNT;
    aliensCounter = ALIENS_UPDATE_COUNT;
    aliensFireCounter = rand() % MAX_ALIENS_FIRE_COUNT + 1;
    heartbeatCounter = ONE_SECOND_COUNT;
    tankDeathCounter = TANK_DEATH_COUNT;
    ufoUpdateCounter = UFO_UPDATE_COUNT;
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
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_AXI_AC97_0_INTERRUPT_MASK);
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

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

void setUfoExplosionCounter() {
    ufoExplosionCounter = UFO_EXPLOSION_COUNT;
}

void resetUfoAppearanceCounter() {
    int temp = (UFO_APPEAR_COUNT_MAXIMUM - UFO_APPEAR_COUNT_MINIMUM);
    ufoAppearCounter = UFO_APPEAR_COUNT_MINIMUM + (rand() % temp);
}
