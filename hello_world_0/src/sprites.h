/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

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

#endif /* SPRITES_H_ */
