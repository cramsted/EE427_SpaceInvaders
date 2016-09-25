/*
 * sprites.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "sprites.h"
#include <stdio.h>

//Sprite dimensions
#define TANK_HEIGHT 8
#define TANK_WIDTH 15
#define BUNKER_HEIGHT 18
#define BUNKER_WIDTH 24
#define ALIEN_HEIGHT 8
#define ALIEN_WIDTH 12
#define XPADDING 8
#define YPADDING 16

//Colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF

extern const int saucer_16x7[];
extern const int alien_explosion_12x10[];
extern const int alien_top_in_12x8[];
extern const int alien_top_out_12x8[];
extern const int alien_middle_in_12x8[];
extern const int alien_middle_out_12x8[];
extern const int alien_bottom_in_12x8[];
extern const int alien_bottom_out_12x8[];
extern const int tank_15x8[];
extern const int bunker_24x18[];
extern const int bunkerDamage0_6x6[];
extern const int bunkerDamage1_6x6[];
extern const int bunkerDamage2_6x6[];
extern const int bunkerDamage3_6x6[];

const int* alien_sprites[] = { alien_top_in_12x8, alien_top_out_12x8,
		alien_middle_in_12x8, alien_middle_out_12x8, alien_bottom_in_12x8,
		alien_bottom_out_12x8 };

Sprite initSprite(int height, int width, uint32_t color, const int *sprite) {
	Sprite sp;
	sp.Color.color = color;
	sp.height = height;
	sp.width = width;
	sp.sprite = sprite;
	return sp;
}

Position initPosition(int x, int y) {
	Position p;
	p.x = x;
	p.y = y;
	return p;
}

Tank initTank(int x, int y) {
	Tank t;
	t.lives = 3;
	t.p = initPosition(x, y);
	t.sp = initSprite(TANK_HEIGHT, TANK_WIDTH, GREEN, tank_15x8);
	return t;
}

Bunker initBunker(int x, int y) {
	Bunker b;
	b.erosionLevel = 0;
	b.p = initPosition(x, y);
	b.sp = initSprite(BUNKER_HEIGHT, BUNKER_WIDTH, GREEN, bunker_24x18);
	return b;
}


Alien initAlien(int x, int y, alien_type_e type) {
	Alien a;
	a.alive = 1;
	a.p = initPosition(x, y);
	a.sp = initSprite(ALIEN_HEIGHT, ALIEN_WIDTH, WHITE, alien_sprites[type]);
	return a;
}

Aliens initAliens(int x, int y){
	Aliens a;
	a.numActiveBullets = 0;
	int row, col;
	const int alien_width = 2 * ALIEN_WIDTH + XPADDING;
	const int alien_height = 2 * ALIEN_HEIGHT + YPADDING;
	for(row = 0; row < ALIENS_ROW; row++){
		for(col = 0; col < ALIENS_COL; col++){
			alien_type_e type;
			if (row == 0){
				type = top_alien_in;
			}else if(row >= 1 && row < 3){
				type = middle_alien_in;
			}else{
				type = bottom_alien_in;
			}
			a.aliens[row][col] = initAlien(ALIENS_START_X + (col * alien_width),
					ALIENS_START_Y + (row * alien_height), type);
		}
	}
	return a;
}
