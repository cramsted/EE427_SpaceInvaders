/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "bunkers.h"	//for when the aliens touch the bunkers
#include ".h"			//for ending the game when the aliens get to the
#include "render.h"		//for access to the frame buffer
#include "text.h"		//for access to the function updateScore()
#include <stdio.h>		//xil_printf for debugging purposes
#include "timer.h"		//for setting alien updates and explosion timers

//values that determine the dimensions of the sprites on the screen
#define ALIEN_HEIGHT (8*2)		//alien sprite height
#define ALIEN_WIDTH (12*2)		//alien sprite width
#define XALIEN_PADDING (8)		//padding between aliens in the x direction
#define YALIEN_PADDING (16)		//padding between aliens in the y direction
#define RIGHT_PADDING 12			//padding for the max value the aliens can be drawn in the x direciton
#define ALIENS_BLOCK_WIDTH ((ALIEN_WIDTH + XALIEN_PADDING) * ALIENS_COL)	//size of the alien block
#define MAX_X (SCREEN_WIDTH - RIGHT_PADDING - ALIEN_WIDTH) //max x value that the alien block can travel to
#define MIN_X RIGHT_PADDING //min x value that the alien block can travel to
#define MAX_Y 400 //max y value that the alien block can travel to
#define LEFT_SCREEN_X (SCREEN_WIDTH >> 2) //value for determining if the alien block is hitting the left edge of the screen

//pouint32_t values for different alien types
#define TOP_ALIEN_POINTS 40
#define MIDDLE_ALIEN_POINTS 20
#define BOTTOM_ALIEN_POINTS 10

#define MAX_EXPLOSION_SPRITES 3	//largest number of aliens allowed to be exploding at once

//all the sprite structures defined in sprite_bit_maps.c
extern const uint32_t saucer_16x7[]; //saucer sprite, not used in this lab
extern const uint32_t alien_explosion_12x10[]; //alien explosion spirte
extern const uint32_t alien_top_in_12x8[]; //top row alien sprite with legs in
extern const uint32_t alien_top_out_12x8[]; //top row alien sprite with legs out
extern const uint32_t alien_middle_in_12x8[]; //middle row alien sprite with legs in
extern const uint32_t alien_middle_out_12x8[]; //middle row alien sprite with legs out
extern const uint32_t alien_bottom_in_12x8[]; //bottom row alien sprite with legs in
extern const uint32_t alien_bottom_out_12x8[]; //bottom row alien sprite with legs out
extern const uint32_t alien_explosion_12x8[]; //exploding alien sprite

//struct of pointers to all alien sprites for easy reference by index value
const int* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Aliens aliens;	//alien block object
static Alien *explodedAliens[MAX_EXPLOSION_SPRITES] = { NULL, NULL, NULL }; //stores the current aliens that are displaying the explosion sprite

// Function prototypes - see descriptions in function definitions
void eraseAlien(Alien *alien);
void bunkerCollisionCheck(uint32_talienX, uint32_t alienY);
uint32_t aliensAt();
void findAndErodeBunkerBlock(uint32_t x, uint32_t y, Bunker *temp);
void checkPointCollision(uint32_t x, uint32_t y);

//initialize an alien struct
//param x sets starting x position
//param y sets starting y position
//param type sets the type and guise of the alien
//param row sets the row the alien will be in
static void initAlien(uint32_t x, uint32_t y, uint32_t row, uint32_t col, alien_type_e type) {
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
void initAliens(uint32_t x, uint32_t y) {
	aliens.numActiveBullets = 0; //inits the number of active alien bullets to 0
	aliens.direction = right; //starts alien block moving right on the screen
	uint32_t row, col;
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
	uint32_t i;
	for (i = 0; i < ALIENS_COL; i++) { //sets which aliens are on the front row of the alien block
		aliens.frontRowAliens[i] = &aliens.aliens[ALIENS_ROW - 1][i];
	}

	// Keeps track of how many aliens are killed
	aliens.killCount = 0;
}

//adds an alien to the explodedAlien buffer
void addExplodedAlienSprite(Alien *alien) {
	// Add the alien to the list of explosion sprites so they can be erased
	uint8_t i;
	for (i = 0; i < MAX_EXPLOSION_SPRITES; i++) {
		if (explodedAliens[i] == NULL) {
			explodedAliens[i] = alien;
		}
	}
}

//erases of an exploded alien sprite if it hasn't already been erased
void eraseAlienExplosionSprite() {
	uint8_t i;
	for (i = 0; i < MAX_EXPLOSION_SPRITES; i++) {
		if (explodedAliens[i] != NULL) {
			eraseAlien(explodedAliens[i]);
			explodedAliens[i] = NULL;
		}
	}
}

//erases the passed in alien by changing it's sprite color to match the background
void eraseAlien(Alien *alien) {
	alien->sp.Color.color = BLACK;
	editFrameBuffer(&alien->sp, &alien->p); //makes change in the frame buffer
}

//draws an exploding alien sprite
void explodeAlien(Alien *alien) {
	alien->sp.sprite = alien_explosion_12x8;
	editFrameBuffer(&alien->sp, &alien->p);
	addExplodedAlienSprite(alien);
}

//sets the status var of the alien at the passed in row and col as dead
void killAlien(Alien *alien, uint32_t row, uint32_t col) {
	alien->status = exploded;
	explodeAlien(alien); //removes alien from the screen
	setAlienExplosionCounter();
	aliens.killCount++;

	// adjust frontRowAliens, which keeps track of which aliens can fire a bullet
	if (row != 0) {
		//makes sure that the alien next in line is alive
		uint32_t i = row - 1;
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

//draws a passed in alien at the designated x,y coordinate
void drawAlien(uint32_t xUpdate, uint32_t yUpdate, Alien *alien) {
	//checks if the alien is currently exploding. If so, it set the alien to dead
	if (alien->status == exploded) {
		alien->status = dead;
		eraseAlien(alien);
	}
	else if (alien->status == alive) {
		// erase the alien and update its position
		eraseAlien(alien);
		alien->p.x += xUpdate;
		alien->p.y += yUpdate;

		// change the type from out to in or vice versa
		(alien->type & 1) ? alien->type-- : alien->type++;
		alien->sp.sprite = alien_sprites[alien->type];

		//check to see if a bunker needs to be erased prior to drawing the aliens over it.
		if ((alien->p.y + ALIEN_HEIGHT) >= BUNKER_START_Y) {
			checkPointCollision(alien->p.x, (alien->p.y + ALIEN_HEIGHT));
			checkPointCollision((alien->p.x + ALIEN_WIDTH), (alien->p.y + ALIEN_HEIGHT));
		}

		// redraw the alien
		alien->sp.Color.color = WHITE;
		editFrameBuffer(&alien->sp, &alien->p);
	}
}

//draws the block of aliens at the specified x,y coordinate on the screen
void drawAliens(uint32_t xUpdate, uint32_t yUpdate) {
	uint32_t row, col;
	if (aliens.direction == left) { // moving left
		for (row = 0; row < ALIENS_ROW; row++) {
			for (col = 0; col < ALIENS_COL; col++) {
				Alien *temp = &aliens.aliens[row][col];
				drawAlien(xUpdate, yUpdate, temp);
			}
		}
	} else { // moving right or down
		for (row = ALIENS_ROW - 1; row >= 0; row--) {
			for (col = ALIENS_COL - 1; col >= 0; col--) {
				Alien *temp = &aliens.aliens[row][col];
				drawAlien(xUpdate, yUpdate, temp);
			}
		}
	}
}

// We want to find the leftmost column with living aliens
// Used for bounds checking when the aliens are moving left
Position aliensLeftBlockPosition() {
	uint32_t row, col;
	for (col = 0; col < ALIENS_COL; col++) {
		for (row = 0; row < ALIENS_ROW; row++) {
			//finds and returns furthest left most alive alien
			if (aliens.aliens[row][col].status == alive)
				return aliens.aliens[row][col].p;
		}
	}
	//if no aliens are alive, then the upper left coordinate of the alien block is returned
	return initPosition(ALIENS_START_X, ALIENS_START_Y);
}

// Same as the aliensLeftBlockPosition, but used for bounds checking on the right
Position aliensRightBlockPosition() {
	uint32_t row, col;
	for (col = ALIENS_COL - 1; col >= 0; col--) {
		for (row = ALIENS_ROW - 1; row >= 0; row--) {
			//finds and returns furthest right most alive alien
			if (aliens.aliens[row][col].status == alive)
				return aliens.aliens[row][col].p;
		}
	}
	//if no aliens are alive, then the upper left coordinate of the alien block is returned
	return initPosition(ALIENS_START_X, ALIENS_START_Y);
}

//moves the alien block a predetermined distance and direction
void updateAliens() {
	Position p;
	//takes the current direction the alien block is moving as input
	switch (aliens.direction) {
	case left:
		drawAliens(left, 0);
		//finds the position of the left most alien and checks that it isn't past the min x value
		p = aliensLeftBlockPosition();
		if (p.x < MIN_X) {
			aliens.direction = down; //shifts the aliens down
		}
		break;
	case right:
		drawAliens(right, 0);
		//finds the position of the right most alien and checks that it isn't past the max x value
		p = aliensRightBlockPosition();
		if (p.x > MAX_X) {
			aliens.direction = down; //shifts the aliens down
		}
		break;
	case down:
		drawAliens(0, down);
		//checks if the aliens have reached the tank
		if (aliensAt()) {
			//ends the game and writes 'game over' to the screen
			drawGameOver();
			while (1);
		}
		//determins where the alien block is on the screen an then sets it's direction appropriately
		p = aliensLeftBlockPosition();
		if (p.y < MAX_Y) {
			if (p.x < LEFT_SCREEN_X) {
				aliens.direction = right;
			} else {
				aliens.direction = left;
			}
		}
		break;
	default:
		break;
	}

	// End the game AFTER updating the aliens, which does ensure all sprites are
	// erased, if all the aliens are dead.
	if (aliens.killCount == ALIENS_ROW * ALIENS_COL) {
		// end game
		drawGameOver();
		while (1);
	}
}

//test code
//returns 1 if the passed in x,y coordinate is inside the passed in bunker bit map; returns 0 otherwise
static uint32_t alienCollidesWithBunker(uint32_t alienX, uint32_t alienY, Bunker *bunker) {
	//pointers to the bunkers position and sprite for convenience.
	Position *spritePos = &bunker->p;
	Sprite *sprite = &bunker->sp;
	//calculates the max and min x,y values of the tank sprite
	uint32_t spriteX = spritePos->x;
	uint32_t spriteY = spritePos->y;
	uint32_t spriteXMax = spriteX + sprite->width;
	uint32_t spriteYMax = spriteY + sprite->height;
	//calculates the max values of the alien sprite. Min values are not needed since they are passed in.
	uint32_t alienXMax = alienX + ALIEN_WIDTH;
	uint32_t alienYMax = alienY + ALIEN_HEIGHT;

	//checks for overlapping on the bottom left of the alien sprite
	if ((alienX >= spriteX) && (alienX <= spriteXMax)) {
		if ((alienYMax >= spriteY) && (alienYMax <= spriteYMax)) {
			return 1;
		}
	}

	//checks for overlapping on the bottom right of the alien sprite
	if ((alienXMax >= spriteX) && (alienXMax <= spriteXMax)) {
		if ((alienYMax >= spriteY) && (alienYMax <= spriteYMax)) {
			return 1;
		}
	}
	return 0; //bunker miss
}

//test code originally used to test if the alien block was touching a bunker
void bunkerCollisionCheck(uint32_t alienX, uint32_t alienY) {
	uint32_t i;
	for (i = 0; i < MAX_BUNKERS; i++) {
		if (bunkers.bunkers[i].sp.Color.color != BLACK) {
			//erases the entire bunker if the x,y values are found inside the sprite
			if (alienCollidesWithBunker(alienX, alienY, &bunkers.bunkers[i])) {
				destroyWholeBunker(&bunkers.bunkers[i]);
			}
		}
	}
}

//checks if the aliens have reached the bottom of the screen
uint32_t aliensAt() {
	uint32_t i, maxY = 0;
	//checks the array that keeps track of the aliens on the front row
	for (i = 0; i < ALIENS_COL; i++) {
		//checks if the alien is alive
		if (aliens.frontRowAliens[i]->status == alive) {
			if (maxY <= aliens.frontRowAliens[i]->p.y) {
				//keeps track of the alien that is lowest in the alien block
				maxY = aliens.frontRowAliens[i]->p.y;
			}
		}
	}
	//check if the alien is past the max y value
	if ((maxY + ALIEN_HEIGHT) >= ALIENS_START_Y) {
		return 1; //aliens have won
	}
	return 0; //aliens haven't won yet
}

//checks if there is a collision between a sprite and the bunker
//erodes the portion of the bunker that has been struck
void checkPointCollision(uint32_t x, uint32_t y) {
	uint32_t i;
	for (i = 0; i < MAX_BUNKERS; i++) {
		Bunker *temp = &bunkers.bunkers[i];
		uint32_t bunkerX = temp->p.x;
		uint32_t bunkerY = temp->p.y;
		uint32_t bunkerXMax = bunkerX + temp->sp.width;
		uint32_t bunkerYMax = bunkerY + temp->sp.height;
		//determine if the x,y pouint32_t is inside the bunker
		if ((x >= bunkerX) && (x <= bunkerXMax)) {
			if ((y >= bunkerY) && (y <= bunkerYMax)) {
				//erodes the appropriate portion of the bunker
				findAndErodeBunkerBlock(x, y, temp);
			}
		}
	}
}

//sets the value of a section of bunker to 'gone' if the x,y value is inside of it
void findAndErodeBunkerBlock(uint32_t x, uint32_t y, Bunker *temp) {
	uint32_t bunkerX = temp->p.x;
	uint32_t bunkerY = temp->p.y;
	//find the size of the erosion rows and cols in the bunker
	uint32_t colSize = BUNKER_WIDTH / EROSION_COLS;
	uint32_t rowSize = BUNKER_HEIGHT / EROSION_ROWS;
	//test which block the x,y pouint32_t is in
	uint32_t row, col;
	//checks all of the sections of the bunker for a collision with the x,y coordinate
	for (row = EROSION_ROWS - 1; row >= 0; row--) {
		for (col = EROSION_COLS - 1; col >= 0; col--) {
			if (x >= (bunkerX + colSize * col)) {
				if (y >= (bunkerY + rowSize * row)) {
					eraseBunkerSection(temp, row, col);
					return;
				}
			}
		}
	}
}
