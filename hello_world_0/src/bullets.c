/*
 * bullets.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bullets.h"
#include "render.h"		//for edit_frameBuffer
#include <stdlib.h>
#include <time.h>			//for random number generator

//values that determine the dimensions of the sprites on the screen
#define BULLET_WIDTH (3 * 2)	//sprite width
#define BULLET_HEIGHT (5 * 2)	//sprite height
#define ALIEN_BULLETS_UPDATE_Y 5	//alien bullet speed
#define TANK_BULLET_UPDATE_Y (-ALIEN_BULLETS_UPDATE_Y)	//tank bullet speed
#define BULLET_MIN_Y 45	//min y position of a bullet
#define BULLET_MAX_Y 440	//max y position of a bullet

//all the sprite structures defined in sprite_bit_maps.c
extern const int bulletCross_3x5[];
extern const int bulletLightning_3x5[];
//used for keeping track of number of active bullets fired by aliens
extern Aliens aliens;

//creates an initialized bullet struct
Bullet initBullet(const int *sprite) {
	Bullet b;
	b.active = 0;	//init to inactive
	b.sp = initSprite(BULLET_HEIGHT, BULLET_WIDTH, WHITE, sprite);
	return b;
}

//creates the containing bullet struct
Bullets initBullets() {
	// seed random number generator
	srand(time(NULL));

	Bullets b;
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (i & 1) {	//odd array index numbers have lightning sprites
			b.bullets[i] = initBullet(bulletLightning_3x5);
		} else {	//even array index number have cross sprites
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
	edit_frameBuffer(&bullet->sp, &bullet->p);	//update frame buffer
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
	if (b->active) {	//allows for only one tank bullet at a time
		return;
	}
	//sets bullet position based on the current tank position
	b->p.x = tank->p.x + (tank->sp.width / 2) - (b->sp.width / 2);
	b->p.y = tank->p.y - b->sp.height - 1;
	b->active = 1;
	drawBullet(b, TANK_BULLET_UPDATE_Y);
}

// Initialize and fire an alien bullet
void alienPew(Aliens *aliens, Bullets *bullets) {
	//TODO: marshall I don't know what this var does here or in the if statement at the end
	static int alien_turn = 0;

	//limits number of bullets aliens can have active at one time
	if (aliens->numActiveBullets >= MAX_BULLETS) {
		return;
	}

	// Find a random alien
	do {
		alien_turn = rand() % ALIENS_COL;
	} while (aliens->frontRowAliens[alien_turn]->status == dead);
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
