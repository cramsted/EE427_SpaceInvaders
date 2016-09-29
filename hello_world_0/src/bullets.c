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
#define ALIEN_BULLETS_UPDATE_Y 5
#define TANK_BULLET_UPDATE_Y (-ALIEN_BULLETS_UPDATE_Y)
#define BULLET_MIN_Y 45
#define BULLET_MAX_Y 440

extern const int bulletCross_3x5[];
extern const int bulletLightning_3x5[];
extern Aliens aliens;

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

// Erase the bullet from the screen
void eraseBullet(Bullet *bullet) {
	bullet->sp.Color.color = BLACK;
	edit_frameBuffer(&bullet->sp, &bullet->p);
}

// Update the bullet's y position and draw it on the screen
void drawBullet(Bullet *bullet, int updateY) {
	bullet->sp.Color.color = WHITE;
	bullet->p.y += updateY;
	edit_frameBuffer(&bullet->sp, &bullet->p);
}

// Update all tank and alien bullet positions
void updateBullets(Bullets *bullets) {
	int i;
	Bullet *b = bullets->bullets;

	// tank bullet
	if (b[0].active) {
		eraseBullet(&b[0]);
		// Delete the bullet if it has left the screen
		if (b[0].p.y <= BULLET_MIN_Y) {
			b[0].active = 0;
			return;
		}
		drawBullet(&b[0], TANK_BULLET_UPDATE_Y);
	}

	// alien bullets
	for (i = 1; i < MAX_BULLETS; i++) {
		if (b[i].active) {
			eraseBullet(&b[i]);
			// Delete the bullet if it has left the screen
			if (b[i].p.y >= BULLET_MAX_Y) {
				b[i].active = 0;
				aliens.numActiveBullets--;
			} else {
				drawBullet(&b[i], ALIEN_BULLETS_UPDATE_Y);
			}
		}
	}
}

// Initialize and fire the tank bullet
void tankPew(Tank *tank, Bullets *bullets) {
	Bullet *b = &bullets->bullets[0];
	if (b->active) {
		return;
	}
	b->p.x = tank->p.x + (tank->sp.width / 2) - (b->sp.width / 2);
	b->p.y = tank->p.y - b->sp.height - 1;
	b->active = 1;
	drawBullet(b, TANK_BULLET_UPDATE_Y);
}

// Initialize and fire an alien bullet
void alienPew(Aliens *aliens, Bullets *bullets) {
	static int alien_turn = 0;
	if (aliens->numActiveBullets >= MAX_BULLETS) {
		return;
	}

	// Find a random alien
	Alien *a = aliens->frontRowAliens[alien_turn];
	aliens->numActiveBullets++;

	// Find an inactive bullet
	Bullet *b = (void*)0;
	Bullet *bulletList = bullets->bullets;
	int i;
	for (i = 1; i < MAX_BULLETS; i++) {
		if (!bulletList[i].active) {
			b = &bulletList[i];
			break;
		}
	}

	// Initialize the bullet position and draw
	b->p.x = a->p.x + (a->sp.width / 2) - (b->sp.width / 2);
	b->p.y = a->p.y + a->sp.height + 1;
	b->active = 1;

	if (++alien_turn >= ALIENS_COL) {
		alien_turn = 0;
	}
	drawBullet(b, ALIEN_BULLETS_UPDATE_Y);
}
