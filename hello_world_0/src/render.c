/*
 * render.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "render.h"
#include "sprites.h"

#define MAX_ALIENS 55
#define MAX_BUNKERS 4
#define MAX_BULLETS 5 // one for tank and four for aliens

typedef struct {
	int x;
	int y;
	int alive;
	int *sprite;
} Alien;

typedef struct {
	int numActiveBullets;
	Alien aliens[MAX_ALIENS];
} Aliens;

typedef struct {
	int x;
	int lives;
	int *sprite;
} Tank;

typedef struct {
	int x;
	int erosionLevel;
	int *sprite;
} Bunker;

typedef struct {
	int x;
	int y;
	int *sprite;
} Bullet;

typedef struct {
	Bullet bullets[MAX_BULLETS];
} Bullets;

typedef struct {
	int x;
	int y;
} TankBullet;

static Aliens aliens;
static Tank tank;
static Bunker bunkers[MAX_BUNKERS];

void init() {
	// TODO: initialize various structs, etc.
	// init alien positions and draw them
	// init tank position and lives and draw it
	// init bunker positions and erosion and draw them
	// init score
}

int findPixelValue(int x, int y, int col, int row, int *sprite){
	int xval = (col - x) >> 1;
	int yval = (row - y) >> 1;

	//check to make sure we haven't gone out of bounds
//	if(yval >= size){
//		return -1;
//	}

	return (sprite[yval] << xval) & 0x80000000;
}
