/*
 * globals.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>

Tank tank;
Aliens aliens;
Bunkers bunkers;
Bullets bullets;

void tankPew() {
	Bullet *b = &bullets.bullets[0];
	b->p.x = tank.p.x + (tank.sp.width / 2);
	b->p.y = tank.p.y - b->sp.height - 1;
	b->active = 1;
}

void alienPew() {
	static int alien_turn = 0;
	if (aliens.numActiveBullets >= MAX_BULLETS) {
		return;
	}
	Alien *a = aliens.frontRowAliens[alien_turn];

	// increment before using as index so we don't use a bullet that's already active
	aliens.numActiveBullets++;
	Bullet *b = &bullets.bullets[aliens.numActiveBullets];
	b->p.x = a->p.x + (a->sp.width/2);
	b->p.y = a->p.y + a->sp.height + 1;
	b->active = 1;

	xil_printf("new alien bullet (x,y) = (%d,%d)\n\r", b->p.x, b->p.y);

	if (++alien_turn >= ALIENS_COL) {
		alien_turn = 0;
	}
}
