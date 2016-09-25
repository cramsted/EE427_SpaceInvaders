/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h>

#define MAX_ALIENS 55
#define MAX_BUNKERS 4
#define MAX_BULLETS 5 // one for tank and four for aliens
extern const int saucer_16x7[];
extern const int alien_explosion_12x10[];
extern const int alien_top_in_12x8[];
extern const int alien_top_out_12x8[];
extern const int alien_middle_in_12x8[];
extern const int alien_middle_out_12x8[];
extern const int alien_bottom_in_12x8[];
extern const int alien_bottom_out_12x8[];
extern const int tank_15x8[];
extern const int bunker_24x18[];
extern const int bunkerDamage0_6x6[];
extern const int bunkerDamage1_6x6[];
extern const int bunkerDamage2_6x6[];
extern const int bunkerDamage3_6x6[];

//meta data for the sprites
typedef struct {
	int height;
	int width;
	union {
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t padding;
		} RGB;
		uint32_t color;
	} Color;
	const int *sprite;
} Sprite;

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	Position p;
	int alive;
	Sprite sp;
} Alien;

typedef struct {
	int numActiveBullets;
	Alien aliens[MAX_ALIENS];
} Aliens;

typedef struct {
	Position p;
	int lives;
	Sprite sp;
} Tank;

typedef struct {
	Position p;
	int erosionLevel;
	Sprite sp;
} Bunker;

typedef struct {
	Position p;
	Sprite sp;
} Bullet;

typedef struct {
	Bullet bullets[MAX_BULLETS];
} Bullets;

typedef struct {
	Position p;
} TankBullet;

Tank initTank(int x, int y);
Position initPosition(int x, int y);

#endif /* SPRITES_H_ */
