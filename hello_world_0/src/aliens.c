/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "bunkers.h"
#include "tank.h"
#include "render.h"		//for access to the frame buffer
#include "text.h"		//for access to the function updateScore()
#include <stdio.h>		//xil_printf for debugging purposes
#include "timer.h"
#include "audio_files/audio.h"

//values that determine the dimensions of the sprites on the screen
#define ALIEN_HEIGHT (8*2)		//alien sprite height
#define ALIEN_WIDTH (12*2)		//alien sprite width
#define XALIEN_PADDING (8)		//padding between aliens in the x direction
#define YALIEN_PADDING (16)		//padding between aliens in the y direction
#define RIGHT_PADDING 12			//padding for the max value the aliens can be drawn in the x direciton
#define ALIENS_BLOCK_WIDTH ((ALIEN_WIDTH + XALIEN_PADDING) * ALIENS_COL)
#define MAX_X (SCREEN_WIDTH - RIGHT_PADDING - ALIEN_WIDTH) //fix me!
#define MIN_X RIGHT_PADDING
#define MAX_Y 400
#define LEFT_SCREEN_X (SCREEN_WIDTH >> 2)
//point values for different alien types
#define TOP_ALIEN_POINTS 40
#define MIDDLE_ALIEN_POINTS 20
#define BOTTOM_ALIEN_POINTS 10

#define MAX_EXPLOSION_SPRITES 3

//all the sprite structures defined in sprite_bit_maps.c
extern const int32_t saucer_16x7[]; //saucer sprite, not used in this lab
extern const int32_t alien_explosion_12x10[]; //alien explosion spirte
extern const int32_t alien_top_in_12x8[]; //top row alien sprite with legs in
extern const int32_t alien_top_out_12x8[]; //top row alien sprite with legs out
extern const int32_t alien_middle_in_12x8[]; //middle row alien sprite with legs in
extern const int32_t alien_middle_out_12x8[]; //middle row alien sprite with legs out
extern const int32_t alien_bottom_in_12x8[]; //bottom row alien sprite with legs in
extern const int32_t alien_bottom_out_12x8[]; //bottom row alien sprite with legs out
extern const int32_t alien_explosion_12x8[]; //exploding alien sprite

//struct of pointers to all alien sprites for easy reference by index value
const int32_t* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Aliens aliens;
static Alien *explodedAliens[MAX_EXPLOSION_SPRITES] = { NULL, NULL, NULL };

// Function prototypes
void eraseAlien(Alien *alien);
void bunkerCollisionCheck(int32_t alienX, int32_t alienY);
int32_t aliensAtTank();
void findAndErodeBunkerBlock(int32_t x, int32_t y, Bunker *temp);
void checkPointCollision(int32_t x, int32_t y);
//initialize an alien struct
//param x sets starting x position
//param y sets starting y position
//param type sets the type and guise of the alien
//param row sets the row the alien will be in
static void initAlien(int32_t x, int32_t y, int32_t row, int32_t col, alien_type_e type) {
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
void initAliens(int32_t x, int32_t y) {
	aliens.numActiveBullets = 0; //inits the number of active alien bullets to 0
	aliens.direction = right; //starts alien block moving right on the screen
	int32_t row, col;
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
	int32_t i;
	for (i = 0; i < ALIENS_COL; i++) { //sets which aliens are on the front row of the alien block
		aliens.frontRowAliens[i] = &aliens.aliens[ALIENS_ROW - 1][i];
	}

	// Keeps track of how many aliens are killed
	aliens.killCount = 0;
}

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
void killAlien(Alien *alien, int32_t row, int32_t col) {
	alien->status = exploded;
	explodeAlien(alien); //removes alien from the screen
	setAlienExplosionCounter();
	aliens.killCount++;

	// adjust frontRowAliens, which keeps track of which aliens can fire a bullet
	if (row != 0) {
		//makes sure that the alien next in line is alive
		int32_t i = row - 1;
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

void drawAlien(int32_t xUpdate, int32_t yUpdate, Alien *alien) {
	if (alien->status == exploded) {
		alien->status = dead;
		eraseAlien(alien);
	} else if (alien->status == alive) {
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
void drawAliens(int32_t xUpdate, int32_t yUpdate) {
	int32_t row, col;
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
	int32_t row, col;
	for (col = 0; col < ALIENS_COL; col++) {
		for (row = 0; row < ALIENS_ROW; row++) {
			if (aliens.aliens[row][col].status == alive)
				return aliens.aliens[row][col].p;
		}
	}
	return initPosition(ALIENS_START_X, ALIENS_START_Y);
}

// Same as the aliensLeftBlockPosition, but used for bounds checking on the right
Position aliensRightBlockPosition() {
	int32_t row, col;
	for (col = ALIENS_COL - 1; col >= 0; col--) {
		for (row = ALIENS_ROW - 1; row >= 0; row--) {
			if (aliens.aliens[row][col].status == alive)
				return aliens.aliens[row][col].p;
		}
	}
	return initPosition(ALIENS_START_X, ALIENS_START_Y);
}

//moves the alien block a predetermined distance and direction
void updateAliens() {
    setAudioEvent(AUDIO_ALIEN_MOVEMENT);
	Position p;
	switch (aliens.direction) {
	case left:
		drawAliens(left, 0);
		p = aliensLeftBlockPosition();
		if (p.x < MIN_X) {
			aliens.direction = down;
		}
		break;
	case right:
		drawAliens(right, 0);
		p = aliensRightBlockPosition();
		if (p.x > MAX_X) {
			aliens.direction = down;
		}
		break;
	case down:
		drawAliens(0, down);
		if (aliensAtTank()) {
			drawGameOver();
			while (1)
				;
		}
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

static int32_t alienCollidesWithBunker(int32_t alienX, int32_t alienY, Bunker *bunker) {
	Position *spritePos = &bunker->p;
	Sprite *sprite = &bunker->sp;
	int32_t spriteX = spritePos->x;
	int32_t spriteY = spritePos->y;
	int32_t spriteXMax = spriteX + sprite->width;
	int32_t spriteYMax = spriteY + sprite->height;
	int32_t alienXMax = alienX + ALIEN_WIDTH;
	int32_t alienYMax = alienY + ALIEN_HEIGHT;

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

void bunkerCollisionCheck(int32_t alienX, int32_t alienY) {
	int32_t i;
	for (i = 0; i < MAX_BUNKERS; i++) {
		if (bunkers.bunkers[i].sp.Color.color != BLACK) {
			if (alienCollidesWithBunker(alienX, alienY, &bunkers.bunkers[i])) {
				destroyWholeBunker(&bunkers.bunkers[i]);
			}
		}
	}
}
int32_t aliensAtTank() {
	int32_t i, maxY = 0;
	for (i = 0; i < ALIENS_COL; i++) {
		if (aliens.frontRowAliens[i]->status == alive) {
			if (maxY <= aliens.frontRowAliens[i]->p.y) {
				maxY = aliens.frontRowAliens[i]->p.y;
			}
		}
	}
	if ((maxY + ALIEN_HEIGHT) >= TANK_START_Y) {
		return 1; //aliens have won
	}
	return 0; //aliens haven't won yet
}

// check if a bullet hit a sprite
// return tank_bullet_hit or alien_bullet_hit if true, no_hit otherwise
void alienCollidesWithBunkers(Alien *alien, Position *p) {
	//check bottom left for collision
	//if yes, erode that section of the bunker
	//else
	//	if (!bullet->active) {
	//		return 0; //false because the bullet is not on the screen
	//	}
	//	int32_t spriteX = spritePos->x;
	//	int32_t spriteY = spritePos->y;
	//	int32_t spriteXMax = spriteX + sprite->width;
	//	int32_t spriteYMax = spriteY + sprite->height;
	//	int32_t bulletX = bullet->p.x;
	//	int32_t bulletY = bullet->p.y;
	//	int32_t bulletXMax = bulletX + BULLET_WIDTH;
	//	int32_t bulletYMax = bulletY + BULLET_HEIGHT;
	//
	//	//checks for overlapping on the top left of the bullet sprite
	//	if ((bulletX >= spriteX) && (bulletX <= spriteXMax)) {
	//		if ((bulletY >= spriteY) && (bulletY <= spriteYMax)) {
	//			return tank_bullet_hit;
	//		}
	//	}
	//	//checks for overlapping on the bottom right of the bullet sprite
	//	if ((bulletXMax >= spriteX) && (bulletXMax <= spriteXMax)) {
	//		if ((bulletYMax >= spriteY) && (bulletYMax <= spriteYMax)) {
	//			return alien_bullet_hit;
	//		}
	//	}
	//	return no_hit;
}

void checkPointCollision(int32_t x, int32_t y) {
	int32_t i;
	for (i = 0; i < MAX_BUNKERS; i++) {
		Bunker *temp = &bunkers.bunkers[i];
		int32_t bunkerX = temp->p.x;
		int32_t bunkerY = temp->p.y;
		int32_t bunkerXMax = bunkerX + temp->sp.width;
		int32_t bunkerYMax = bunkerY + temp->sp.height;
		//determine if the x,y point is inside the bunker
		if ((x >= bunkerX) && (x <= bunkerXMax)) {
			if ((y >= bunkerY) && (y <= bunkerYMax)) {
				findAndErodeBunkerBlock(x, y, temp);
			}
		}
	}
}

void findAndErodeBunkerBlock(int32_t x, int32_t y, Bunker *temp) {
	int32_t bunkerX = temp->p.x;
	int32_t bunkerY = temp->p.y;
	//find the size of the erosion rows and cols in the bunker
	int32_t colSize = BUNKER_WIDTH / EROSION_COLS;
	int32_t rowSize = BUNKER_HEIGHT / EROSION_ROWS;
	//test which block the x,y point is in
	int32_t row, col;
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
