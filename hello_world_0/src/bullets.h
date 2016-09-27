/*
 * bullets.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BULLETS_H_
#define BULLETS_H_

#include "sprites.h"

#define MAX_BULLETS 5 // one for tank and four for aliens

typedef struct {
	Position p;
	Sprite sp;
	// 0 inactive, 1 is active
	uint8_t active;
} Bullet;

typedef struct {
	Bullet bullets[MAX_BULLETS];
} Bullets;

void updateBullets(Bullets *bullets);
Bullets initBullets();


#endif /* BULLETS_H_ */
