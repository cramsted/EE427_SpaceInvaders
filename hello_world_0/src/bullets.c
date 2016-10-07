/*
 * bullets.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bullets.h"
#include "render.h"		//for edit_frameBuffer
#include "aliens.h"
#include "bunkers.h"
#include <stdlib.h>

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

Bullets bullets;

static int bulletCollidesWithSprite(Bullet *bullet, Sprite *sprite,
		Position *spritePos);
static void checkTankBulletCollisions();
static void destroyBullet(Bullet *bullet);
static void alienHit(Bullet *bullet);
//creates an initialized bullet struct
Bullet initBullet(const int *sprite) {
	Bullet b;
	b.active = 0; //init to inactive
	b.sp = initSprite(BULLET_HEIGHT, BULLET_WIDTH, WHITE, sprite);
	return b;
}

//creates the containing bullet struct
Bullets initBullets() {
	Bullets b;
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (i & 1) { //odd array index numbers have lightning sprites
			b.bullets[i] = initBullet(bulletLightning_3x5);
		} else { //even array index number have cross sprites
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
	edit_frameBuffer(&bullet->sp, &bullet->p); //update frame buffer
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
	checkTankBulletCollisions();
}

// Initialize and fire the tank bullet
void tankPew(Tank *tank, Bullets *bullets) {
	Bullet *b = &bullets->bullets[0];
	if (b->active) { //allows for only one tank bullet at a time
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

	//limits number of bullets aliens can have active at one time
	if (aliens->numActiveBullets >= MAX_ALIEN_BULLETS) {
		return;
	}

	// Find a random alien
	int alien_turn = 0;
	do {
		alien_turn = rand() % ALIENS_COL;
	} while (aliens->frontRowAliens[alien_turn]->status == dead);
	Alien *a = aliens->frontRowAliens[alien_turn];
	aliens->numActiveBullets++;

	// Find an inactive bullet
	Bullet *b = (void*) 0;
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

	drawBullet(b, ALIEN_BULLETS_UPDATE_Y);
}

void bunkerHit(Bullet *bullet) {
	int i;
	for (i = 0; i < MAX_BUNKERS; i++) {
		Bunker *bunker = &bunkers.bunkers[i];
		if (bulletCollidesWithSprite(bullet, &bunker->sp, &bunker->p)) {
			erodeBunker(bunker, 0, 0); //TODO: compute the bunker section
			destroyBullet(bullet);
		}
	}

}

static void alienHit(Bullet *bullet) {
	int row, col;
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			Alien *alien = &aliens.aliens[row][col];
			if (bulletCollidesWithSprite(bullet, &alien->sp, &alien->p)) {
				if (alien->status == alive) {
					killAlien(alien, row, col); //TODO: alien explosion
					destroyBullet(bullet);
				}
			}
		}
	}

}

static void tankHit(Bullet *bullet) {
	if (bulletCollidesWithSprite(bullet, &tank.sp, &tank.p)) {
		tankExplode(); //TODO: tank explosion
		destroyBullet(bullet);

	}
}

static void destroyBullet(Bullet *bullet) {
	eraseBullet(bullet);
	bullet->active = 0; //deactivates the bullet;
}

static int bulletCollidesWithSprite(Bullet *bullet, Sprite *sprite,
		Position *spritePos) {
	if (!bullet->active) {
		return 0; //false because the bullet is not on the screen
	}
	int spriteX = spritePos->x;
	int spriteY = spritePos->y;
	int spriteXMax = spriteX + sprite->width;
	int spriteYMax = spriteY + sprite->height;
	int bulletX = bullet->p.x;
	int bulletY = bullet->p.y;
	int bulletXMax = bulletX + BULLET_WIDTH;
	int bulletYMax = bulletY + BULLET_HEIGHT;

	//checks for overlapping on the left side of the bullet sprite
	if ((bulletX >= spriteX) && (bulletX <= spriteXMax)) {
		if ((bulletY >= spriteY) && (bulletY <= spriteYMax)) {
			return 1; //true
		}
	}
	//checks for overlapping on the right side of the bullet sprite
	if ((bulletXMax >= spriteX) && (bulletXMax <= spriteXMax)) {
		if ((bulletYMax >= spriteY) && (bulletYMax <= spriteYMax)) {
			return 1; //true
		}
	}
	return 0; //false
}
void checkTankBulletCollisions() {
	Bullet *tankBullet = &bullets.bullets[0];
	//check bunker
	bunkerHit(tankBullet);
	//check aliens
	alienHit(tankBullet);
}

void checkAlienBulletCollisions() {
	int i;
	for (i = 1; i < MAX_BULLETS; i++) {
		Bullet *alienBullet = &bullets.bullets[i];
		//check bunker
		bunkerHit(alienBullet);
		//check tank
		tankHit(alienBullet);
	}
}
