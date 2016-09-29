/*
 * aliens.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef ALIENS_H_
#define ALIENS_H_

#include "sprites.h"

#define ALIENS_START_X 8
#define ALIENS_START_Y 50
#define ALIENS_COL 11
#define ALIENS_ROW 5

typedef enum {
	top_alien_in = 0, // 0
	top_alien_out, // 1
	middle_alien_in, // 2
	middle_alien_out, // 3
	bottom_alien_in, // 4
	bottom_alien_out // 5
} alien_type_e;


typedef enum {
	left, down, right
} alien_direction_e;

typedef enum {
	dead, alive
}alien_life_status_e;

typedef struct {
	Position p;
	Sprite sp;
	alien_type_e type;
	alien_life_status_e status;
	uint8_t row;
} Alien;

typedef struct {
	Alien aliens[ALIENS_ROW][ALIENS_COL];
	Alien* frontRowAliens[ALIENS_COL];
	uint8_t numActiveBullets;
	alien_direction_e direction;
} Aliens;

Aliens initAliens(int x, int y);
void drawAliens(int x, int y, Aliens *aliens);
void updateAliens(Aliens *aliens);
void killAlien(Aliens *aliens, int row, int col);

#endif /* ALIENS_H_ */