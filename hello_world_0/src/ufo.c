/*
 * ufo.c
 *
 *  Created on: Oct 10, 2016
 *      Author: superman
 */

#include "ufo.h"
#include "timer.h"
#include "render.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>

#define UFO_MAX_POINTS 350

// The UFO object
UFO ufo;

// UFO sprites
extern const uint32_t saucer_16x7[];
extern const uint32_t ufo_explosion_16x7[];

//funciton prototypes
void eraseUfo();

// Gets changed whenever the UFO is killed; keeps track of how many points the UFO is worth
uint32_t ufoPoints;

// Initialize the ufo in the chosen position
void initUfo() {
	ufo.p = initPosition(UFO_START_X_LEFT, UFO_START_Y);
	ufo.sp = initSprite(UFO_HEIGHT, UFO_WIDTH, RED, saucer_16x7);
    
    // We're starting on the left side of the screen, so begin going right
	ufo.direction = ufo_right;
    
    // The UFO shouldn't start out drawn on the screen
	ufo.status = inactive;
}

//moves the ufo in the direction of ufo_direction_e by a preset number of pixels
void updateUfo() {
	// check if it is active
	// if it is, erase it, update its position, and redraw it
	// collision with bullets is done in bullet.c
	if (ufo.status == active) {
		drawUfo();

		// If the ufo is off the screen, make it disappear
		if ((ufo.p.x >= UFO_START_X_RIGHT) || ufo.p.x <= UFO_START_X_LEFT) {
			ufoDisapear();
		}
	}
}

// Kills the ufo - involves:
//  erasing the UFO,
//  making the UFO be inactive (so it can't be shot again),
//  assigning random points,
//  drawing the points on the screen where the UFO was,
//  setting a counter that, when it expires, will cause the points to be erased,
//  and updating the score
void killUfo() {
	eraseUfo();
	ufo.status = inactive;
	ufoPoints = rand() % UFO_MAX_POINTS;
	drawUfoPoints(&ufo.p, ufoPoints, RED);

	// set a timer to wait to erase the explosion
	setUfoExplosionCounter();
	updateScore(ufoPoints);
}

// Erase the UFO from the screen
void eraseUfo() {
	ufo.sp.Color.color = BLACK;
	editFrameBuffer(&ufo.sp, &ufo.p);

	// This sets the amount of time until the ufo reappears
	resetUfoAppearanceCounter();
}

// Erase the ufo,
// Update its position (the amount of pixels is embedded in the direction)
// Redraw
void drawUfo() {
	eraseUfo();
	ufo.sp.Color.color = RED;
	ufo.p.x += ufo.direction;
	editFrameBuffer(&ufo.sp, &ufo.p);
}

// Called when the UFO exits the screen; we make the UFO
// switch directions, erase the UFO, and make it inactive
// (so it can't be shot again)
void ufoDisapear() {
	(ufo.direction == ufo_right) ? (ufo.direction = ufo_left) : (ufo.direction
			= ufo_right);
	eraseUfo();
	drawUfoPoints(&ufo.p, ufoPoints, BLACK);
	ufo.status = inactive;
}

// Draw the UFO at the correct position, with the correct sprite
// Make it active
void ufoAppear() {
	//	xil_printf("a wild ufo appears!");
	if (ufo.direction == ufo_right) {
		// draw at the top-left
		ufo.p = initPosition(UFO_START_X_LEFT, UFO_START_Y);
	} else {
		// draw at the top-right
		ufo.p = initPosition(UFO_START_X_RIGHT, UFO_START_Y);
	}
	ufo.status = active;
	ufo.sp.sprite = saucer_16x7;
	drawUfo();
}
