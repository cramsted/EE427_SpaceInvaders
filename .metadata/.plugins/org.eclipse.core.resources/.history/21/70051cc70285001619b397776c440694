/*
 * bullets.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bullets.h"

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
