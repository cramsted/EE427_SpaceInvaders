/*
 * aliens.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#ifndef ALIENS_H_
#define ALIENS_H_

//for acces to the Sprites and Position objects
#include "sprites.h"

//positional and size constraints on the aliens block
#define ALIENS_START_X 8			//starting x position of the alien block
#define ALIENS_START_Y 50			//starting y position of the alien block
#define ALIENS_COL 11					//number of aliens in a row
#define ALIENS_ROW 5					//number of aliens in a column

//custom variable set for easily changing the guise of any type of alien sprites
typedef enum {
	top_alien_in = 0, // 0
	top_alien_out, // 1
	middle_alien_in, // 2
	middle_alien_out, // 3
	bottom_alien_in, // 4
	bottom_alien_out // 5
} alien_type_e;

//possible directions of movement for the alien block
typedef enum {
	left, down, right
} alien_direction_e;

//possible life statuses of alien structs
typedef enum {
	dead, alive
}alien_life_status_e;

//struct for all needed info to draw a single alien
typedef struct {
	Position p; 										//x,y position of the sprite
	Sprite sp;											//sprite object
	alien_type_e type;							//what sprite type the alien is and what guise it is in
	alien_life_status_e status;			//alive or dead
	uint8_t row;										//what row the alien is in. Used for adjusting end point
																	//of the alien block when one of the end cols is destroyed
} Alien;

//struct for all needed info to draw the block of aliens
typedef struct {
	Alien aliens[ALIENS_ROW][ALIENS_COL];				//an array (row x col) of all the aliens in the block
	Alien* frontRowAliens[ALIENS_COL];					//array keeping track of what aliens are on the front
																							//row. Used for calculating position of bullets fired
																							//by aliens.
	uint8_t numActiveBullets;										//number of active bullets fired by aliens
	alien_direction_e direction;								//what direction the block of aliens is currently traveling in
} Aliens;

extern Aliens aliens;

//function to initialize the alien block struct
// param x sets starting x position
//param y sets starting y position
Aliens initAliens(int x, int y);
//draws the block of aliens at the inputted x,y coordinate
void drawAliens(int x, int y, Aliens *aliens);
//moves the alien block in the direction of alien_direction_e by a preset number of pixels
void updateAliens(Aliens *aliens);
//kills the alien at the specified row and col of the alien block
void killAlien(Aliens *aliens, int row, int col);

#endif /* ALIENS_H_ */
