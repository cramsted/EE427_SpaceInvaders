/*
 * tank.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "tank.h"
#include "render.h"
#include <stdio.h>

//values that determine the dimensions of the sprites on the screen
#define TANK_HEIGHT (8*2)	//tank sprite height
#define TANK_WIDTH (15*2)	//tank sprite width
#define PIXELS_PER_MOVE 3	//tank move rate
#define RIGHT_PADDING 4	//padding on right side of screen
#define LEFT_PADDING RIGHT_PADDING //ditto for left side of screen

//tank sprites
extern const uint32_t tank_15x8[];
extern const uint32_t tank_explosion_15x8[];
extern const uint32_t tank_explosion2[];

//tank object
Tank tank;

//creates a tank struct initialized to position x,y
Tank initTank(uint32_t x, uint32_t y) {
	Tank t;
	t.lives = MAX_LIVES;
	t.p = initPosition(x, y);
	t.sp = initSprite(TANK_HEIGHT, TANK_WIDTH, GREEN, tank_15x8);
	return t;
}

//draws a tank at position x. The y position is fixed
void drawTank(uint32_t x, Tank *tank) {
	tank->sp.Color.color = BLACK; //draws black over old tank position
	editFrameBuffer(&tank->sp, &tank->p);
	tank->sp.Color.color = GREEN;
	tank->p.x = x;
	editFrameBuffer(&tank->sp, &tank->p); //draws tank at new position
}

//redraws the tank sprite a predetermined distance to the right
void moveTankRight(Tank *tank) {
	uint32_t currentX = tank->p.x;
	//checks if tank is at the max right value
	if (currentX + PIXELS_PER_MOVE + TANK_WIDTH + RIGHT_PADDING > SCREEN_WIDTH)
		return;
	drawTank(currentX + PIXELS_PER_MOVE, tank);
}

//redraws the tank sprite a predetermined distance to the left
void moveTankLeft(Tank *tank) {
	uint32_t currentX = tank->p.x;
	//checks if the tank is a the max left value
	if (currentX - PIXELS_PER_MOVE - LEFT_PADDING < 0)
		return;
	drawTank(currentX - PIXELS_PER_MOVE, tank);
}

#define DELAY 500000
inline void shortDelay() {
	volatile uint32_t i = DELAY;
	while (i--);
}

void tankExplode() {
	// draw the tank explosion sprite
	uint32_t i;
	for (i = 10; i > 0; --i) {
		tank.sp.sprite = tank_explosion_15x8;
		editFrameBuffer(&tank.sp, &tank.p);
		shortDelay();
		tank.sp.sprite = tank_explosion2;
		editFrameBuffer(&tank.sp, &tank.p);
		shortDelay();
	}

	// reset the tank sprite back to its normal sprite because
	// we want to draw the normal tank when we resume gameplay
	tank.sp.sprite = tank_15x8;

	// lose a life
	if (tank.lives > 0) {
		tank.lives--;

		// Redrawing lives will automatically erase one from the screen.
		drawLives();
	}
}

//draws the lives in the upper right hand corner of screen.
//coordinate values are fixed
void drawLives() {
	Tank life = initTank(LIFE_START_X, LIFE_START_Y); //creates a new tank life
	uint32_t col;
	for (col = 0; col < MAX_LIVES; col++) {
		if (col < tank.lives) { //draws lives if the col num is less than the num of lives left
			life.sp.Color.color = GREEN;
		} else {
			life.sp.Color.color = BLACK; //draws lives black if col num is >= num of lives left
		}
		life.p.x = LIFE_START_X + (col * XLIFE_PADDING) + col * life.sp.width; //calculates life x position
		editFrameBuffer(&life.sp, &life.p); //updates frame buffer
	}
}
