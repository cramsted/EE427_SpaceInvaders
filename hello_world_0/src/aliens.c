/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "render.h"

#define ALIEN_HEIGHT (8*2)
#define ALIEN_WIDTH (12*2)
#define XALIEN_PADDING (8)
#define YALIEN_PADDING (16)

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
	a.sp = initSprite(ALIEN_HEIGHT, ALIEN_WIDTH, WHITE, alien_sprites[type]);
	return a;
}

Aliens initAliens(int x, int y) {
	Aliens a;
	a.numActiveBullets = 0;
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
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			Alien *temp = &aliens->aliens[row][col];
			temp->sp.Color.color = BLACK;
			edit_frameBuffer(&temp->sp, &temp->p);
			temp->sp.Color.color = WHITE;
			temp->p.x = x + (temp->sp.width + 8) * col;
			temp->p.y = y + (temp->sp.height + 16) * row;
			edit_frameBuffer(&temp->sp, &temp->p);
		}
	}
}
