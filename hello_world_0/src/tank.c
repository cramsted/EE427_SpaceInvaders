/*
 * tank.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "tank.h"
#include "render.h"

//values that determine the dimensions of the sprites on the screen
#define TANK_HEIGHT (8*2)	//tank sprite height
#define TANK_WIDTH (15*2)	//tank sprite width
#define PIXELS_PER_MOVE 4	//tank move rate
#define RIGHT_PADDING 4	//padding on right side of screen
#define LEFT_PADDING RIGHT_PADDING //ditto for left side of screen

//tank sprite
extern const int tank_15x8[];

Tank tank;

//creates a tank struct initialized to position x,y
Tank initTank(int x, int y) {
	Tank t;
	t.lives = MAX_LIVES;
	t.p = initPosition(x, y);
	t.sp = initSprite(TANK_HEIGHT, TANK_WIDTH, GREEN, tank_15x8);
	return t;
}

//draws a tank at position x. The y value is hard coded
void drawTank(int x, Tank *tank) {
	tank->sp.Color.color = BLACK;	//draws black over old tank position
	edit_frameBuffer(&tank->sp, &tank->p);
	tank->sp.Color.color = GREEN;
	tank->p.x = x;
	edit_frameBuffer(&tank->sp, &tank->p);	//draws tank at new position
}

//redraws the tank sprite a predetermined distance to the right
void moveTankRight(Tank *tank){
	int currentX = tank->p.x;
	//checks if tank is at the max right value
	if (currentX + PIXELS_PER_MOVE + TANK_WIDTH + RIGHT_PADDING > SCREEN_WIDTH)
		return;
	drawTank(currentX + PIXELS_PER_MOVE, tank);
}

//redraws the tank sprite a predetermined distance to the left
void moveTankLeft(Tank *tank){
	int currentX = tank->p.x;
	//checks if the tank is a the max left value
	if (currentX - PIXELS_PER_MOVE - LEFT_PADDING < 0)
		return;
	drawTank(currentX - PIXELS_PER_MOVE, tank);
}

void tankExplode(){
	// TODO: set explode sprite
	// lose a life
	// redraw lives
}

//draws the lives in the upper right hand corner of screen.
//coordinate values are hard coded
//TODO: use number of remaining lives to correctly draw
void drawLives(Tank *tank) {
	Tank life = initTank(LIFE_START_X, LIFE_START_Y); //creates a new tank life
	int col;
	for (col = 0; col < MAX_LIVES; col++) {
		if (col < tank->lives) { //draws lives if the col num is less than the num of lives left
			tank->sp.Color.color = GREEN;
		} else {
			tank->sp.Color.color = BLACK; //draws lives black if col num is >= num of lives left
		}
		life.p.x = LIFE_START_X + (col * XLIFE_PADDING) + col * life.sp.width;	//calculates life x position
		edit_frameBuffer(&life.sp, &life.p);	//updates frame buffer
	}
}
