/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "render.h"
#include <stdio.h>

#define ALIEN_HEIGHT (8*2)
#define ALIEN_WIDTH (12*2)
#define XALIEN_PADDING (8)
#define YALIEN_PADDING (16)
#define ALIENS_SHIFT_X 20
#define ALIENS_SHIFT_Y 16
#define RIGHT_PADDING 8

extern const int saucer_16x7[];
extern const int alien_explosion_12x10[];
extern const int alien_top_in_12x8[];
extern const int alien_top_out_12x8[];
extern const int alien_middle_in_12x8[];
extern const int alien_middle_out_12x8[];
extern const int alien_bottom_in_12x8[];
extern const int alien_bottom_out_12x8[];

const int* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Alien initAlien(int x, int y, alien_type_e type, uint8_t row) {
	Alien a;
	a.status = alive;
	a.row = row;
	a.p = initPosition(x, y);
	a.type = type;
	a.sp = initSprite(ALIEN_HEIGHT, ALIEN_WIDTH, WHITE, alien_sprites[type]);
	return a;
}

Aliens initAliens(int x, int y) {
	Aliens a;
	a.numActiveBullets = 0;
	a.direction = right;
	int row, col;
	const int alien_width = ALIEN_WIDTH + XALIEN_PADDING;
	const int alien_height = ALIEN_HEIGHT + YALIEN_PADDING;
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			alien_type_e type;
			if (row == 0) {
				type = top_alien_in;
			} else if (row >= 1 && row < 3) {
				type = middle_alien_in;
			} else {
				type = bottom_alien_in;
			}
			a.aliens[row][col] = initAlien(
					ALIENS_START_X + (col * alien_width),
					ALIENS_START_Y + (row * alien_height), type, row);
		}
	}
	return a;
}

void eraseAlien(Alien *alien) {
	alien->sp.Color.color = BLACK;
	edit_frameBuffer(&alien->sp, &alien->p);
}

void killAlien(Aliens *aliens, int row, int col) {
	Alien *alien = &aliens->aliens[row][col];
	alien->status = dead;
	eraseAlien(alien);
}

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

int findStartAlienCol(Aliens *aliens) {
	int i;
	Alien *a;
	for (i = 0; i < ALIENS_COL; i++) {
		a = &aliens->aliens[0][i];
		if (a->status == alive) {
			return i;
		}
	}
	return 0;
}

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

void updateAliens(Aliens *aliens) {
	// TODO: adjust alien x position when left column is destroyed
	// by using leftColIndex for currx and curry
	int startx = ALIENS_START_X + XALIEN_PADDING + ALIENS_SHIFT_X;
	int leftColIndex = findStartAlienCol(aliens);
	int rightColIndex = findEndAlienCol(aliens);
	int currx = aliens->aliens[0][0].p.x; // use [0][leftColIndex]
	int curry = aliens->aliens[0][0].p.y; // use [0][leftColIndex]
	int endx = SCREEN_WIDTH - RIGHT_PADDING - ALIENS_SHIFT_X - (rightColIndex *
			(aliens->aliens[0][0].sp.width + XALIEN_PADDING));

	xil_printf("startx = %d, endx = %d, "
			"currx = %d, curry = %d, "
			"left col index = %d, right col index = %d\n\r"
			, startx, endx, currx, curry, leftColIndex, rightColIndex);

	// state machine to determine which way to go
	switch (aliens->direction) {
	case left:
		xil_printf("going left.\n\r");
		drawAliens(currx - ALIENS_SHIFT_X, curry, aliens);
		if (currx < startx) {
			aliens->direction = down;
		}
		break;
	case down:
		xil_printf("going down.\n\r");
		drawAliens(currx, curry + ALIENS_SHIFT_Y, aliens);
		if (currx > (SCREEN_WIDTH >> 2)) { // on right side, go left
			aliens->direction = left;
		} else { // on the left side, go right
			aliens->direction = right;
		}
		break;
	case right:
		xil_printf("going right.\n\r");
		drawAliens(currx + ALIENS_SHIFT_X, curry, aliens);
		if (currx > endx) {
			aliens->direction = down;
		}
		break;
	default:
		break;
	}
}