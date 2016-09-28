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
#include <stdio.h>

#define BUNKER_HEIGHT (18*2)
#define BUNKER_WIDTH (24*2)
#define XBUNKER_PADDING (80)

extern const int bunker_24x18[];
extern const int bunkerDamage0_6x6[];
extern const int bunkerDamage1_6x6[];
extern const int bunkerDamage2_6x6[];
extern const int bunkerDamage3_6x6[];

Sprite erosion_1;
Sprite erosion_2;
Sprite erosion_3;
Sprite erosion_4;

Sprite* erosion_sprites[] = { &erosion_1, &erosion_2, &erosion_3, &erosion_4 };

Bunker initBunker(int x, int y) {
	Bunker b;
	memset(b.erosionLevel, none, EROSION_ROWS * EROSION_COLS);
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
	erosion_1 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage0_6x6);
	erosion_2 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage1_6x6);
	erosion_3 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage2_6x6);
	erosion_4 = initSprite(EROSION_HEIGHT_AND_WIDTH, EROSION_HEIGHT_AND_WIDTH,
			GREEN, bunkerDamage3_6x6);
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
void erodeBunker(int bunker, int row, int col) {
	if( row == 2 ){
		if(col == 1 || col == 2){
			return;
		}
	}
	Bunker *b = &bunkers.bunkers[bunker];

	b->erosionLevel[row][col]++;

	// get position
	Position p;
	p.x = b->p.x + (EROSION_HEIGHT_AND_WIDTH * col);
	p.y = b->p.y + (EROSION_HEIGHT_AND_WIDTH * row);

	// erase and redraw
	Sprite *sp = erosion_sprites[b->erosionLevel[row][col]];
	sp->Color.color = BLACK;
	edit_frameBuffer(sp, &p);
	sp->Color.color = GREEN;
	edit_frameBuffer(sp, &p);

}
