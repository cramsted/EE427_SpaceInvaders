/*
 * bullets.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bullets.h"
#include "render.h"

#define BULLET_WIDTH (3 * 2)
#define BULLET_HEIGHT (5 * 2)
#define BULLETS_UPDATE_Y 5

extern const int bulletCross_3x5[];
extern const int bulletLightning_3x5[];

Bullet initBullet(const int *sprite) {
	Bullet b;
	b.active = 0;
	b.sp = initSprite(BULLET_HEIGHT, BULLET_WIDTH, WHITE, sprite);
	return b;
}

Bullets initBullets() {
	Bullets b;
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (i & 1) {
			b.bullets[i] = initBullet(bulletLightning_3x5);
		} else {
			b.bullets[i] = initBullet(bulletCross_3x5);
		}
	}
	return b;
}

void updateBullets(Bullets *bullets) {
	int i;
	Bullet *b = bullets->bullets;

	// tank bullet
	if (b[0].active) {
		b[0].sp.Color.color = BLACK;
		edit_frameBuffer(&b[0].sp, &b[0].p);
		b[0].sp.Color.color = WHITE;;
		b[0].p.y -= BULLETS_UPDATE_Y;
		edit_frameBuffer(&b[0].sp, &b[0].p);
	}

	// alien bullets
	for (i = 1; i < MAX_BULLETS; i++) {
		if (b[i].active) {
			b[i].sp.Color.color = BLACK;
			edit_frameBuffer(&b[i].sp, &b[i].p);
			b[i].sp.Color.color = WHITE;
			b[i].p.y += BULLETS_UPDATE_Y;
			edit_frameBuffer(&b[i].sp, &b[i].p);
		}
	}

	// TODO: limits - destroy the bullets when they exit the screen
}

void tankPew(Tank *tank, Bullets *bullets) {
	Bullet *b = &bullets->bullets[0];
	b->p.x = tank->p.x + (tank->sp.width / 2);
	b->p.y = tank->p.y - b->sp.height - 1;
	b->active = 1;
}

void alienPew(Aliens *aliens, Bullets *bullets) {
	static int alien_turn = 0;
	if (aliens->numActiveBullets >= MAX_BULLETS) {
		return;
	}
	Alien *a = aliens->frontRowAliens[alien_turn];

	// increment before using as index so we don't use a bullet that's already active
	aliens->numActiveBullets++;
	Bullet *b = &bullets->bullets[aliens->numActiveBullets];
	b->p.x = a->p.x + (a->sp.width / 2);
	b->p.y = a->p.y + a->sp.height + 1;
	b->active = 1;

	if (++alien_turn >= ALIENS_COL) {
		alien_turn = 0;
	}
}
