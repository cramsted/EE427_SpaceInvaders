/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "render.h"		//for access to the frame buffer
#include "text.h"		//for access to the function updateScore()
#include <stdio.h>		//xil_printf for debugging purposes
#include "timer.h"

//values that determine the dimensions of the sprites on the screen
#define ALIEN_HEIGHT (8*2)		//alien sprite height
#define ALIEN_WIDTH (12*2)		//alien sprite width
#define XALIEN_PADDING (8)		//padding between aliens in the x direction
#define YALIEN_PADDING (16)		//padding between aliens in the y direction
#define ALIENS_SHIFT_X 10		//number of pixels the aliens shift once in the x direction
#define ALIENS_SHIFT_Y 16		//number of pixels the aliens shift once in the y direction
#define RIGHT_PADDING 8			//padding for the max value the aliens can be drawn in
//the x direciton

//point values for different alien types
#define TOP_ALIEN_POINTS 40
#define MIDDLE_ALIEN_POINTS 20
#define BOTTOM_ALIEN_POINTS 10

//all the sprite structures defined in sprite_bit_maps.c
extern const int saucer_16x7[]; //saucer sprite, not used in this lab
extern const int alien_explosion_12x10[]; //alien explosion spirte
extern const int alien_top_in_12x8[]; //top row alien sprite with legs in
extern const int alien_top_out_12x8[]; //top row alien sprite with legs out
extern const int alien_middle_in_12x8[]; //middle row alien sprite with legs in
extern const int alien_middle_out_12x8[]; //middle row alien sprite with legs out
extern const int alien_bottom_in_12x8[]; //bottom row alien sprite with legs in
extern const int alien_bottom_out_12x8[]; //bottom row alien sprite with legs out
extern const int alien_explosion_12x8[]; //exploding alien sprite

//struct of pointers to all alien sprites for easy reference by index value
const int* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Aliens aliens;

Alien *explodedAlien = NULL;

// Function prototypes
void eraseAlien(Alien *alien);

//initialize an alien struct
//param x sets starting x position
//param y sets starting y position
//param type sets the type and guise of the alien
//param row sets the row the alien will be in
static void initAlien(int x, int y, int row, int col, alien_type_e type) {
	Alien *a = &aliens.aliens[row][col];
	a->status = alive; //set life status
	a->p = initPosition(x, y); //set initial x,y position
	a->type = type; //set alien sprite type

	//initializes the sprite to an alien
	a->sp = initSprite(ALIEN_HEIGHT, ALIEN_WIDTH, WHITE, alien_sprites[type]);
}

//function to initialize an alien struct
// param x sets starting x position
//param y sets starting y position
void initAliens(int x, int y) {
	aliens.numActiveBullets = 0; //inits the number of active alien bullets to 0
	aliens.direction = right; //starts alien block moving right on the screen
	int row, col;
	const uint32_t alien_width = ALIEN_WIDTH + XALIEN_PADDING; //width of of sprite plus padding
	const uint32_t alien_height = ALIEN_HEIGHT + YALIEN_PADDING; //height of sprite plus padding

	//populates the aliens[][] struct with alien structs
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			alien_type_e type;
			if (row == 0) { //puts top aliens in top row
				type = top_alien_in;
			} else if (row >= 1 && row < 3) { //puts middle aliens in middle row
				type = middle_alien_in;
			} else { //puts bottom aliens in bottom row
				type = bottom_alien_in;
			}

			//initializes the alien with the correct x,y coordinate and type
			initAlien(ALIENS_START_X + (col * alien_width),
					ALIENS_START_Y + (row * alien_height), row, col, type);
		}
	}

	// Create a list of pointers to aliens that are on the front row
	// so it's easy to keep track of which aliens can fire a bullet.
	int i;
	for (i = 0; i < ALIENS_COL; i++) { //sets which aliens are on the front row of the alien block
		aliens.frontRowAliens[i] = &aliens.aliens[ALIENS_ROW - 1][i];
	}
}

//erases of an exploded alien sprite if it hasn't already been erased
void eraseAlienExplosionSprite() {
	if (explodedAlien != NULL) {
		eraseAlien(explodedAlien);
		explodedAlien = NULL;
	}
}

//erases the passed in alien by changing it's sprite color to match the background
void eraseAlien(Alien *alien) {
	alien->sp.Color.color = BLACK;
	editFrameBuffer(&alien->sp, &alien->p); //makes change in the frame buffer
	explodedAlien = NULL;
}

//draws an exploding alien sprite
void explodeAlien(Alien *alien) {
	alien->sp.sprite = alien_explosion_12x8;
	editFrameBuffer(&alien->sp, &alien->p);
	explodedAlien = alien;
}

//sets the status var of the alien at the passed in row and col as dead
void killAlien(Alien *alien, int row, int col) {
	alien->status = dead;
	explodeAlien(alien); //removes alien from the screen
	setAlienExplosionCounter();

	// adjust frontRowAliens, which keeps track of which aliens can fire a bullet
	if (row != 0) {
		//makes sure that the alien next in line is alive
		int i = row - 1;
		while ((aliens.aliens[i][col].status == dead) && (i > 0)) {
			i--;
		}
		aliens.frontRowAliens[col] = &aliens.aliens[i][col];
	}

	// update the score
	switch (alien->type) {
	case top_alien_in:
	case top_alien_out:
		updateScore(TOP_ALIEN_POINTS);
		break;
	case middle_alien_in:
	case middle_alien_out:
		updateScore(MIDDLE_ALIEN_POINTS);
		break;
	case bottom_alien_in:
	case bottom_alien_out:
		updateScore(BOTTOM_ALIEN_POINTS);
		break;
	default:
		break;
	}
}

//draws the block of aliens at the specified x,y coordinate on the screen
void drawAliens(int x, int y, Aliens *aliens) {
	int row, col;
	if (aliens->direction == left) { // moving left
		for (row = 0; row < ALIENS_ROW; row++) {
			for (col = 0; col < ALIENS_COL; col++) {
				Alien *temp = &aliens->aliens[row][col];

				if (temp->status == alive) {
					// erase the alien and update its position
					eraseAlien(temp);
					temp->p.x = x + (temp->sp.width + XALIEN_PADDING) * col;
					temp->p.y = y + (temp->sp.height + YALIEN_PADDING) * row;

					// change the type from out to in or vice versa
					(temp->type & 1) ? temp->type-- : temp->type++;
					temp->sp.sprite = alien_sprites[temp->type];

					// redraw the alien
					temp->sp.Color.color = WHITE;
					editFrameBuffer(&temp->sp, &temp->p);
				}
			}
		}
	} else { // moving right or down
		for (row = ALIENS_ROW - 1; row >= 0; row--) {
			for (col = ALIENS_COL - 1; col >= 0; col--) {
				Alien *temp = &aliens->aliens[row][col];

				if (temp->status == alive) {
					// erase the alien and update its position
					eraseAlien(temp);
					temp->p.x = x + (temp->sp.width + XALIEN_PADDING) * col;
					temp->p.y = y + (temp->sp.height + YALIEN_PADDING) * row;

					// change the type from out to in or vice versa
					temp->type & 1 ? temp->type-- : temp->type++;
					temp->sp.sprite = alien_sprites[temp->type];

					// redraw the alien
					temp->sp.Color.color = WHITE;
					editFrameBuffer(&temp->sp, &temp->p);
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
	int col, row;
	Alien *a;
	for (col = ALIENS_COL - 1; col >= 0; col--) {
		for( row = ALIENS_ROW - 1; row >= 0; row--){
			a = &aliens->aliens[row][col];
			if (a->status == alive) {
				//the index is 1-11 because it is used when multiplying the width of the alien
				return col + 1;
			}
		}
	}
	return 0;
}

//moves the alien block a predetermined distance and direction
void updateAliens(Aliens *aliens) {
	// TODO: adjust alien x position when left column is destroyed
	// by using leftColIndex for currx and curry
	int startx = ALIENS_START_X + XALIEN_PADDING + ALIENS_SHIFT_X;
	int leftColIndex = findStartAlienCol(aliens); //finds the max value of the left most col
	int rightColIndex = findEndAlienCol(aliens); //finds the max value of the right most col
	int currx = aliens->aliens[0][0].p.x; // use [0][leftColIndex]
	int curry = aliens->aliens[0][0].p.y; // use [0][leftColIndex]
	int endx = SCREEN_WIDTH - RIGHT_PADDING - ALIENS_SHIFT_X - (rightColIndex
			* (aliens->aliens[0][0].sp.width + XALIEN_PADDING)); //largest allowed x value

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
