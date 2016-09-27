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
#define EROSION_WIDTH 	4
#define EROSION_HEIGHT 	5

typedef enum {
	none = 0, level1, level2, level3, dead
} erosion_e;

typedef struct {
	Position p;
	Sprite sp;
	uint8_t erosionLevel[EROSION_HEIGHT][EROSION_WIDTH];
} Bunker;

typedef struct {
	Bunker bunkers[MAX_BUNKERS];
} Bunkers;

Bunkers initBunkers(int x, int y);
void drawBunkers(int x, int y);
void erodeBunker(int bunker);
#endif /* BUNKERS_H_ */
