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

UFO ufo;
extern const int saucer_16x7[];
extern const int ufo_explosion_16x7[];

//funciton prototypes
void eraseUfo();

int ufoPoints;

// Initialize the ufo in the chosen position
void initUfo() {
	ufo.p = initPosition(UFO_START_X_LEFT, UFO_START_Y);
	ufo.sp = initSprite(UFO_HEIGHT, UFO_WIDTH, RED, saucer_16x7);
	ufo.direction = ufo_right;
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

//kills the ufo
void killUfo() {
	eraseUfo();
	ufo.status = inactive;
	ufoPoints = rand() % UFO_MAX_POINTS;
	drawUfoPoints(&ufo.p, ufoPoints, RED);

	// set a timer to wait to erase the explosion
	setUfoExplosionCounter();
	updateScore(ufoPoints);
}

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
