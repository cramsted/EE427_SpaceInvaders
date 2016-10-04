/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "render.h"		//for access to the frame buffer
#include <stdio.h>		//xil_printf for debugging purposes

//values that determine the dimensions of the sprites on the screen
#define ALIEN_HEIGHT (8*2)		//alien sprite height
#define ALIEN_WIDTH (12*2)		//alien sprite width
#define XALIEN_PADDING (8)		//padding between aliens in the x direction
#define YALIEN_PADDING (16)		//padding between aliens in the y direction
#define ALIENS_SHIFT_X 20		//number of pixels the aliens shift once in the x direction
#define ALIENS_SHIFT_Y 16		//number of pixels the aliens shift once in the y direction
#define RIGHT_PADDING 8			//padding for the max value the aliens can be drawn in
								//the x direciton

//all the sprite structures defined in sprite_bit_maps.c
extern const int saucer_16x7[];							//saucer sprite, not used in this lab
extern const int alien_explosion_12x10[];				//alien explosion spirte
extern const int alien_top_in_12x8[];					//top row alien sprite with legs in
extern const int alien_top_out_12x8[];					//top row alien sprite with legs out
extern const int alien_middle_in_12x8[];				//middle row alien sprite with legs in
extern const int alien_middle_out_12x8[];				//middle row alien sprite with legs out
extern const int alien_bottom_in_12x8[];				//bottom row alien sprite with legs in
extern const int alien_bottom_out_12x8[];				//bottom row alien sprite with legs out

//struct of pointers to all alien sprites for easy reference by index value
const int* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Aliens aliens;

//function to initialize an alien struct
//Note: this fucntion is exclusive to the aliens.c file
//param x sets starting x position
//param y sets starting y position
//param alien_type_e sets the type and guise of the alien
//sets the value of what row the alien will be in
Alien initAlien(int x, int y, alien_type_e type, uint8_t row) {
	Alien a;
	a.status = alive;									//set life status
	a.row = row;										//set row number
	a.p = initPosition(x, y);							//set initial x,y position
	a.type = type;										//set alien sprite type

	//initializes the sprite to an alien
	a.sp = initSprite(ALIEN_HEIGHT, ALIEN_WIDTH, WHITE, alien_sprites[type]);
	return a;
}

//function to initialize an alien struct
// param x sets starting x position
//param y sets starting y position
Aliens initAliens(int x, int y) {
	Aliens a;
	a.numActiveBullets = 0;								//inits the number of active alien bullets to 0
	a.direction = right;								//starts alien block moving right on the screen
	int row, col;
	const uint32_t alien_width = ALIEN_WIDTH + XALIEN_PADDING;			//width of of sprite plus padding
	const uint32_t alien_height = ALIEN_HEIGHT + YALIEN_PADDING;		//height of sprite plus padding

	//populates the aliens[][] struct with alien structs
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			alien_type_e type;
			if (row == 0) {								//puts top aliens in top row
				type = top_alien_in;
			} else if (row >= 1 && row < 3) {			//puts middle aliens in middle row
				type = middle_alien_in;
			} else {									//puts bottom aliens in bottom row
				type = bottom_alien_in;
			}

			//initializes the alien with the correct x,y coordinate and type
			a.aliens[row][col] = initAlien(
					ALIENS_START_X + (col * alien_width),
					ALIENS_START_Y + (row * alien_height), type, row);
		}
	}
	return a;
}

//erases the passed in alien by changing it's sprite color to match the background
void eraseAlien(Alien *alien) {
	alien->sp.Color.color = BLACK;
	edit_frameBuffer(&alien->sp, &alien->p);			//makes change in the frame buffer
}

//sets the status var of the alien at the passed in row and col as dead
void killAlien(Aliens *aliens, int row, int col) {
	Alien *alien = &aliens->aliens[row][col];			//finds the alien
	alien->status = dead;
	eraseAlien(alien);									//removes alien from the screen
}

//draws the block of aliens at the specified x,y coordinate on the screen
void drawAliens(int x, int y, Aliens *aliens) {
	int row, col;
	if (aliens->direction == left) { // moving left
		for (row = 0; row < ALIENS_ROW; row++) {
			for (col = 0; col < ALIENS_COL; col++) {
				Alien *temp = &aliens->aliens[row][col];

				// erase the alien and update its position
				eraseAlien(temp);
				temp->p.x = x + (temp->sp.width + XALIEN_PADDING) * col;
				temp->p.y = y + (temp->sp.height + YALIEN_PADDING) * row;

				if (temp->status == alive) {
					// change the type from out to in or vice versa
					(temp->type & 1) ? temp->type-- : temp->type++;
					temp->sp.sprite = alien_sprites[temp->type];

					// redraw the alien
					temp->sp.Color.color = WHITE;
					edit_frameBuffer(&temp->sp, &temp->p);
				}
			}
		}
	} else { // moving right or down
		for (row = ALIENS_ROW - 1; row >= 0; row--) {
			for (col = ALIENS_COL - 1; col >= 0; col--) {
				Alien *temp = &aliens->aliens[row][col];

				// erase the alien and update its position
				eraseAlien(temp);
				temp->p.x = x + (temp->sp.width + XALIEN_PADDING) * col;
				temp->p.y = y + (temp->sp.height + YALIEN_PADDING) * row;

				if (temp->status == alive) {
					// change the type from out to in or vice versa
					temp->type & 1 ? temp->type-- : temp->type++;
					temp->sp.sprite = alien_sprites[temp->type];

					// redraw the alien
					temp->sp.Color.color = WHITE;
					edit_frameBuffer(&temp->sp, &temp->p);
				}
			}
		}
	}
}

// We want to find the leftmost column with living aliens
// Used for bounds checking when the aliens are moving left
int findStartAlienCol(Aliens *aliens) {
	int i;
	Alien *a;
	// Simply loop through the top row of aliens to find an alive one
	for (i = 0; i < ALIENS_COL; i++) {
		a = &aliens->aliens[0][i];
		if (a->status == alive) {
			return i;
		}
	}
	return 0;
}

// Same as the findStartAlienCol, but used for bounds checking on the right
int findEndAlienCol(Aliens *aliens) {
	int i;
	Alien *a;
	for (i = ALIENS_COL - 1; i >= 0; i--) {
		a = &aliens->aliens[0][i];
		if (a->status == alive) {
			return i + 1;
		}
	}
	return 0;
}

//moves the alien block a predetermined distance and direction
void updateAliens(Aliens *aliens) {
	// TODO: adjust alien x position when left column is destroyed
	// by using leftColIndex for currx and curry
	int startx = ALIENS_START_X + XALIEN_PADDING + ALIENS_SHIFT_X;
	int leftColIndex = findStartAlienCol(aliens);	//finds the max value of the left most col
	int rightColIndex = findEndAlienCol(aliens);	//finds the max value of the right most col
	int currx = aliens->aliens[0][0].p.x; 			// use [0][leftColIndex]
	int curry = aliens->aliens[0][0].p.y; 			// use [0][leftColIndex]
	int endx = SCREEN_WIDTH - RIGHT_PADDING - ALIENS_SHIFT_X - (rightColIndex *
			(aliens->aliens[0][0].sp.width + XALIEN_PADDING));	//largest allowed x value

	// state machine to determine which way to go
	switch (aliens->direction) {
	case left:
		drawAliens(currx - ALIENS_SHIFT_X, curry, aliens);
		if (currx < startx) {
			aliens->direction = down;
		}
		break;
	case down:
		drawAliens(currx, curry + ALIENS_SHIFT_Y, aliens);
		if (currx > (SCREEN_WIDTH >> 2)) { // on right side, go left
			aliens->direction = left;
		} else { // on the left side, go right
			aliens->direction = right;
		}
		break;
	case right:
		drawAliens(currx + ALIENS_SHIFT_X, curry, aliens);
		if (currx > endx) {
			aliens->direction = down;
		}
		break;
	default:
		break;
	}
}
