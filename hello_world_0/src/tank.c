/*
 * tank.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "tank.h"
#include "render.h"

#define TANK_HEIGHT (8*2)
#define TANK_WIDTH (15*2)
#define PIXELS_PER_MOVE 4
#define RIGHT_PADDING 4
#define LEFT_PADDING RIGHT_PADDING

extern const int tank_15x8[];

Tank initTank(int x, int y) {
	Tank t;
	t.lives = MAX_LIVES;
	t.p = initPosition(x, y);
	t.sp = initSprite(TANK_HEIGHT, TANK_WIDTH, GREEN, tank_15x8);
	return t;
}

void drawTank(int x, Tank *tank) {
	tank->sp.Color.color = BLACK;
	edit_frameBuffer(&tank->sp, &tank->p);
	tank->sp.Color.color = GREEN;
	tank->p.x = x;
	edit_frameBuffer(&tank->sp, &tank->p);
}

void moveTankRight(Tank *tank){
	int currentX = tank->p.x;
	if (currentX + PIXELS_PER_MOVE + TANK_WIDTH + RIGHT_PADDING > SCREEN_WIDTH)
		return;
	drawTank(currentX + PIXELS_PER_MOVE, tank);
}

void moveTankLeft(Tank *tank){
	int currentX = tank->p.x;
	if (currentX - PIXELS_PER_MOVE - LEFT_PADDING < 0)
		return;
	drawTank(currentX - PIXELS_PER_MOVE, tank);
}

void drawLives(Tank *tank) {
	Tank life = initTank(LIFE_START_X, LIFE_START_Y);
	int col;
	for (col = 0; col < MAX_LIVES; col++) {
		if (col < tank->lives) {
			tank->sp.Color.color = GREEN;
		} else {
			tank->sp.Color.color = BLACK;
		}
		life.p.x = LIFE_START_X + (col * XLIFE_PADDING) + col * life.sp.width;
		edit_frameBuffer(&life.sp, &life.p);
	}
}