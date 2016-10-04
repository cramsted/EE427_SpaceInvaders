/*
 * bullets.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BULLETS_H_
#define BULLETS_H_

#include "sprites.h"		//for accessing the alien sprite
#include "tank.h"				//for accessing the tank postion
#include "aliens.h"			//for accessing the aliens position

#define MAX_BULLETS 5 // one for tank and four for aliens

//info needed to draw a bullet
typedef struct {
	Position p;	// x,y position
	Sprite sp;	// bullet sprite
	uint8_t active; 	// 0 inactive, 1 is active
} Bullet;

//struct for keeping track of all the bullets, both active and inactive
typedef struct {
	Bullet bullets[MAX_BULLETS]; //index 0 is the tank, all others are for aliens
} Bullets;

//updates the position of the bullets, depending on who fired them
void updateBullets(Bullets *bullets);
//inits the bullets struct
Bullets initBullets();
//fires a tank bullet
void tankPew(Tank *tank, Bullets *bullets);
//fires an alien bullet
void alienPew(Aliens *aliens, Bullets *bullets);

#endif /* BULLETS_H_ */
