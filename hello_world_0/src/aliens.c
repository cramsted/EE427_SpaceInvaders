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
	a.alive = 1;
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

void drawAliens(int x, int y, Aliens *aliens) {
	int row, col;
	if (aliens->direction == left) {
		for (row = 0; row < ALIENS_ROW; row++) {
			for (col = 0; col < ALIENS_COL; col++) {
				Alien *temp = &aliens->aliens[row][col];

				// TODO: check for dead alien - don't need to redraw him

				// erase the alien
				temp->sp.Color.color = BLACK;
				edit_frameBuffer(&temp->sp, &temp->p);

				// change the type from out to in or vice versa
				(temp->type & 1) ? temp->type-- : temp->type++;
				temp->sp.sprite = alien_sprites[temp->type];

				// redraw the alien
				temp->sp.Color.color = WHITE;
				temp->p.x = x + (temp->sp.width + 8) * col;
				temp->p.y = y + (temp->sp.height + 16) * row;
				edit_frameBuffer(&temp->sp, &temp->p);
			}

		}
	} else {
		for (row = ALIENS_ROW-1; row >= 0; row--) {
			for (col = ALIENS_COL-1; col >= 0; col--) {
				Alien *temp = &aliens->aliens[row][col];

				// TODO: check for dead alien - don't need to redraw him

				// erase the alien
				temp->sp.Color.color = BLACK;
				edit_frameBuffer(&temp->sp, &temp->p);

				// change the type from out to in or vice versa
				temp->type & 1 ? temp->type-- : temp->type++;
				temp->sp.sprite = alien_sprites[temp->type];

				// redraw the alien
				temp->sp.Color.color = WHITE;
				temp->p.x = x + (temp->sp.width + 8) * col;
				temp->p.y = y + (temp->sp.height + 16) * row;
				edit_frameBuffer(&temp->sp, &temp->p);
			}
		}
	}
}

void updateAliens(Aliens *aliens) {
	// TODO: adjust endx for when a column on the right edge is destroyed
	// TODO: adjust alien x position when left column is destroyed
	// when aliens[0][0] dies, shift the alien we're using right one
	// when aliens[0][ALIENS_COL-1] dies, shift left
	int endx = RIGHT_PADDING + SCREEN_WIDTH -
			((aliens->aliens[0][0].sp.width + XALIEN_PADDING) * ALIENS_COL);
	// TODO: subtract from endx if right col is dead
	int startx = ALIENS_START_X + XALIEN_PADDING; // TODO: add if left col is dead
	int currx = aliens->aliens[0][0].p.x;
	int curry = aliens->aliens[0][0].p.y;

	// state machine to determine which way to go
	switch (aliens->direction) {
	case left:
		drawAliens(currx - ALIENS_SHIFT_X, curry, aliens);
		if (aliens->aliens[0][0].p.x < startx) {
			aliens->direction = down;
		}
		break;
	case down:
		drawAliens(currx, curry + ALIENS_SHIFT_Y, aliens);
		if (aliens->aliens[0][0].p.x > (SCREEN_WIDTH / 2)) { // on right side
			aliens->direction = left;
		} else { // on the left side
			aliens->direction = right;
		}
		break;
	case right:
		drawAliens(currx + ALIENS_SHIFT_X, curry, aliens);
		if (aliens->aliens[0][0].p.x > endx) {
			aliens->direction = down;
		}
		break;
	default:
		break;
	}
}
