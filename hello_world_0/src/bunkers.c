/*
 * bunkers.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "bunkers.h"
#include "globals.h"
#include "render.h"
#include <string.h>

#define BUNKER_HEIGHT (18*2)
#define BUNKER_WIDTH (24*2)
#define XBUNKER_PADDING (80)

extern const int bunker_24x18[];
extern const int bunkerDamage0_6x6[];
extern const int bunkerDamage1_6x6[];
extern const int bunkerDamage2_6x6[];
extern const int bunkerDamage3_6x6[];

const int* erosion_sprites[] = {
		bunkerDamage0_6x6,
		bunkerDamage1_6x6,
		bunkerDamage2_6x6,
		bunkerDamage3_6x6
};

Bunker initBunker(int x, int y) {
	Bunker b;
	memset(b.erosionLevel, none, EROSION_HEIGHT * EROSION_WIDTH);
	b.p = initPosition(x, y);
	b.sp = initSprite(BUNKER_HEIGHT, BUNKER_WIDTH, GREEN, bunker_24x18);
	return b;
}

Bunkers initBunkers(int x, int y) {
	Bunkers b;
	int row;
	const int bunker_width = BUNKER_WIDTH + XBUNKER_PADDING;
	for (row = 0; row < MAX_BUNKERS; row++) {
		b.bunkers[row] = initBunker(BUNKER_START_X + (row * bunker_width), y);
	}
	return b;
}

void drawBunkers(int x, int y) {
	int row;
	for (row = 0; row < MAX_BUNKERS; row++) {
		Bunker *temp = &bunkers.bunkers[row];
		edit_frameBuffer(&temp->sp, &temp->p);
	}
}

// param bunker selects which bunker to erode
// TODO: add section of bunker to erode as parameter when doing collision detecting
void erodeBunker(int bunker) {
	Bunker *b = &bunkers.bunkers[bunker];
	static int row = 0;
	static int col = 0;

	b->erosionLevel[row][col]++;

	// draw it
	Position p;
	p.x = b->p.x + ((col / EROSION_WIDTH) * b->sp.width);
	p.y = b->p.y + ((row / EROSION_HEIGHT) * b->sp.height);

	edit_frameBuffer(erosion_sprites[b->erosionLevel[row][col]], &p);

	// select next section
	if (b->erosionLevel[row][col] == dead) {
		col++;
		if (col >= EROSION_WIDTH) {
			col = 0;
			row++;
			if (row >= EROSION_HEIGHT) {
				row = 0;
			}
		}
	}

}
