/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h>

#define ALIENS_COL 10
#define ALIENS_ROW 5
#define MAX_LIVES 3
#define MAX_BUNKERS 4
#define MAX_BULLETS 5 // one for tank and four for aliens
#define ALIENS_START_X 50
#define ALIENS_START_Y 50
#define BUNKER_START_X 110
#define BUNKER_START_Y 350
#define TANK_START_X 305
#define TANK_START_Y 420
#define LIFE_START_X 450
#define LIFE_START_Y 20
#define XLIFE_PADDING 10
#define GROUND_START_Y 450
//Colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF

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

typedef enum {
	top_alien_in = 0,	// 0
	top_alien_out,		// 1
	middle_alien_in,	// 2
	middle_alien_out,	// 3
	bottom_alien_in,	// 4
	bottom_alien_out	// 5
} alien_type_e;

//TODO: make an enum for bullet types

typedef struct {
	int alive;
	Position p;
	Sprite sp;
	alien_type_e type;
} Alien;

typedef struct {
	int numActiveBullets;
	Alien aliens[ALIENS_ROW][ALIENS_COL];
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
	Bunker bunkers[MAX_BUNKERS];
} Bunkers;

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
Aliens initAliens(int x, int y);
Bunkers initBunkers(int x, int y);
#endif /* SPRITES_H_ */
