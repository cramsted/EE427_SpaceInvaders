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
 *   - alien explosion doesn't always erase
 *   - alien block freezes
 *   - aliens collide with bunkers - destroy the bunkers
 *   - optimize drawing algorithm as needed
 *   - reorganizing: updating functions, removing struct pointers
 *   - CPU utilization
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "platform.h" //needed to setup the FPGA for drawing to a screen
#include "render.h"	//needed for videoInit()
#include "controls.h"	//needed for readInput()
#include "timer.h"
#include "events.h"
#include "xparameters.h"
#include "audio_files/audio.h"

#define START_GAME_DELAY 30000000

int main() {
    // seed random number generator
    srand(time(NULL));
    init_platform(); // Necessary for all programs.
    initAudio();
    initVideoDMAController(); //sets up video hardware
    videoInit(); //initializes the screen to its starting point

    // short delay before things get going
    volatile int delay = START_GAME_DELAY;
    while (--delay)
        ;

    //initializes buttons, the FIT timer, and interrupts
    // Do this AFTER the delay so there's time to draw the screen
    timerInit();

    //	while (1) {
    //		readInput(); //waits for control input
    //	}

    eventsLoop();
    cleanup_platform();

    return 0;
}
