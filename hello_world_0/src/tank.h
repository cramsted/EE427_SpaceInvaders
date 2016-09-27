/*
 * tank.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef TANK_H_
#define TANK_H_

#include "sprites.h"

#define TANK_START_X 305
#define TANK_START_Y 420
#define LIFE_START_X 470
#define LIFE_START_Y 20

typedef struct {
	Position p;
	Sprite sp;
	uint8_t lives;
} Tank;

Tank initTank(int x, int y);
void drawTank(int x, Tank *tank);
void drawLives(Tank *tank);
#endif /* TANK_H_ */
