/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h>

#define CHARACTER_HEIGHT (5 * 2)
#define CHARACTER_WIDTH (8 * 2)

#define ALIENS_COL 10
#define ALIENS_ROW 5
#define MAX_LIVES 3
#define MAX_BUNKERS 4
#define MAX_SCORE_DIGITS 4
#define MAX_CHARACTER_LETTERS 5
#define MAX_BULLETS 5 // one for tank and four for aliens
#define ALIENS_START_X 50
#define ALIENS_START_Y 50
#define BUNKER_START_X 110
#define BUNKER_START_Y 350
#define TANK_START_X 305
#define TANK_START_Y 420
#define LIFE_START_X 470
#define LIFE_START_Y 20
#define SCORE_START_X 20
#define LIVES_START_X 350
#define TEXT_START_Y 25
#define XLIFE_PADDING 10
#define CHARACTER_PADDING 4
#define GROUND_START_Y 450

//Colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define BLACK 0x00000000

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

typedef struct {
	Position p;
	int score;
	Sprite *sp[MAX_SCORE_DIGITS];
} Score;

typedef struct {
	Position p;
	Sprite sp;
} Character;

Character initChar(int x, int y, const int *sprite);
Tank initTank(int x, int y);
Position initPosition(int x, int y);
Aliens initAliens(int x, int y);
Bunkers initBunkers(int x, int y);
#endif /* SPRITES_H_ */
