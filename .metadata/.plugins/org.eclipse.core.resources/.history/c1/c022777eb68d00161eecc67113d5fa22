/*
 * bunkers.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_

#include "sprites.h"	//needed for the Sprite and Position structs

//values that determine the dimensions of the sprites on the screen
#define BUNKER_START_X 	110	//start x value of the bunkers block
#define BUNKER_START_Y 	350	//start y value of all bunkers block
#define MAX_BUNKERS 4	//max number of bunkers
//the bunker sprite has been divided into a grid with these dimensions
#define EROSION_COLS 	4
#define EROSION_ROWS	3
#define EROSION_HEIGHT_AND_WIDTH (6*2)	//erosion block dimentions

//custom set of values to easily change the erosion level of a bunker
typedef enum {
	none = 0, level1, level2, level3, gone
} erosion_e;

//struct containing the needed information to draw a bunker
typedef struct {
	Position p;	//current x,y coordinate
	Sprite sp;	//bunker sprite
	//array to track the erosion level of the different sections of the bunker
	uint8_t erosionLevel[EROSION_ROWS][EROSION_COLS];
} Bunker;

//containing struct for all of the bunkers
typedef struct {
	Bunker bunkers[MAX_BUNKERS];
} Bunkers;

extern Bunkers bunkers;

//creates a new block of bunkers set to position x,y
Bunkers initBunkers(int x, int y);
//draws the block of bunkers at position x,y
void drawBunkers(int x, int y);
//draws an eroded bunker sprite over a portion of the specified bunker
void erodeBunker(Bunker *b, int row, int col);
//erodes all the sections of the bunker
void erodeWholeBunker(int bunker);

#endif /* BUNKERS_H_ */
