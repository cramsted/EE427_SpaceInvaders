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
 *   - aliens adjust when left column dies
 *   - optimize drawing algorithm as needed
 *   - UFO appearing and exploding
 *   - when tank explodes, briefly pause game, then redraw the tank and continue
 *   - reorganizing: updating functions, removing struct pointers
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

#define START_GAME_DELAY 10000000

int main() {
	// seed random number generator
	srand(time(NULL));
	init_platform(); // Necessary for all programs.
	initVideoDMAController(); //sets up video hardware
	videoInit(); //initializes the screen to its starting point
	timerInit(); //initializes buttons, the FIT timer, and interrupts

	// short delay before things get going
	volatile int delay = START_GAME_DELAY;
	while (--delay)
		;

//	while (1) {
//		readInput(); //waits for control input
//	}

	eventsLoop();
	cleanup_platform();

	return 0;
}
