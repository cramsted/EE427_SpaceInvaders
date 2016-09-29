/*
 * bunkers.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_

#include "sprites.h"

#define BUNKER_START_X 	110
#define BUNKER_START_Y 	350
#define MAX_BUNKERS 4
#define EROSION_COLS 	4
#define EROSION_ROWS	3
#define EROSION_HEIGHT_AND_WIDTH (6*2)

typedef enum {
	none = 0, level1, level2, level3, gone
} erosion_e;

typedef struct {
	Position p;
	Sprite sp;
	uint8_t erosionLevel[EROSION_ROWS][EROSION_COLS];
} Bunker;

typedef struct {
	Bunker bunkers[MAX_BUNKERS];
} Bunkers;

Bunkers initBunkers(int x, int y);
void drawBunkers(int x, int y);
void erodeBunker(int bunker, int row, int col);

#endif /* BUNKERS_H_ */
