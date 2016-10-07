/*
 * tank.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef TANK_H_
#define TANK_H_

#include "sprites.h"

//values that determine the dimensions of the sprites on the screen and their starting positions
#define TANK_START_X 305	//tank start x position
#define TANK_START_Y 420	//tank start y position
#define LIFE_START_X 470	//life sprite block start x postion
#define LIFE_START_Y 20		//life sprite block start y position
#define XLIFE_PADDING 10	//padding between life sprites
#define MAX_LIVES 3	//number of lives

//needing info to draw a tank sprite
typedef struct {
	Position p;
	Sprite sp;
	uint8_t lives;
} Tank;

extern Tank tank;

//creates a tank struct initialized to position x,y
Tank initTank(int x, int y);
//draws a tank at position x. The y value is hard coded
void drawTank(int x, Tank *tank);
//draws the lives in the upper right hand corner of screen.
//coordinate values are hard coded
void drawLives(Tank *tank);
//redraws the tank sprite a predetermined distance to the right
void moveTankRight(Tank *tank);
//redraws the tank sprite a predetermined distance to the left
void moveTankLeft(Tank *tank);
//sets the explosion sprite for the tank and erases a life
void tankExplode();
#endif /* TANK_H_ */
