/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * Space Invaders game
 * TODO's:
 *   - bullet collision detection / bunker degradation
 *   - aliens adjust when left column dies
 *   - automate movement and firing
 *   - adjust bottom row of aliens dying
 *   - score
 *   - implement how lives are updated
 *   - optimize drawing algorithm as needed
 *   - controls with push buttons
 *   - alien exploding
 *   - UFO appearing and exploding
 *   - tank exploding
 *   - basic refactoring/reorganizing, e.g. removing globals.c/h
 *   - CPU utilization
 */

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "platform.h" //needed to setup the FPGA for drawing to a screen
#include "render.h"	//needed for videoInit()
#include "controls.h"	//needed for readInput()
#include "timer.h"
#include "events.h"
#include "tank.h"
#include "bullets.h"

// Holds all pending events, where each event is a different bit
uint32_t events = 0;

int main() {
	// seed random number generator
	srand(time(NULL));
	init_platform(); // Necessary for all programs.
	init_videoDMAController(); //sets up video hardware
	videoInit();	//initializes the screen to its starting point
	timerInit();	//initializes buttons, the FIT timer, and interrupts

	// short delay before things get going
	volatile int delay = 100000;
	while (--delay);

	// Events loop:
	while(1){
		if (events) {
			// An event is pending...
			if (events & LEFT_BTN_EVENT) {
				// Move tank left
				events &= ~LEFT_BTN_EVENT;
				moveTankLeft(&tank);
			}
			if (events & RIGHT_BTN_EVENT) {
				// Move tank right
				events &= ~RIGHT_BTN_EVENT;
				moveTankRight(&tank);
			}
			if (events & MIDDLE_BTN_EVENT) {
				// Fire tank bullet
				events &= ~MIDDLE_BTN_EVENT;
				tankPew(&tank, &bullets);
			}
			if (events & BULLETS_REFRESH_EVENT) {
				// Move bullets and do collision detecting
				events &= ~BULLETS_REFRESH_EVENT;
				updateBullets(&bullets);
			}
			if (events & ALIENS_REFRESH_EVENT) {
				// Move aliens
				events &= ~ALIENS_REFRESH_EVENT;
				updateAliens(&aliens);
			}
			if (events & ALIENS_FIRE_EVENT) {
				// Aliens fire a bullet
				events &= ~ALIENS_FIRE_EVENT;
				alienPew(&aliens, &bullets);
			}
		} else {
			// "Idle" event
		}
//		readInput();	//waits for control input
	}

	cleanup_platform();

	return 0;
}
