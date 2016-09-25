/*
 * sprites.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "sprites.h"

//Sprite dimensions
#define TANK_HEIGHT 8
#define TANK_WIDTH 15
#define BUNKER_HEIGHT 18
#define BUNKER_WIDTH 24

//Colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF

//macros for the sprite pixel arrays
#define packword32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |						  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

const int saucer_16x7[] =
{
	packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
	packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
	packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
	packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

const int alien_explosion_12x10[] =
{
	packword12(0,0,0,0,0,0,1,0,0,0,0,0),
	packword12(0,0,0,1,0,0,1,0,0,0,1,0),
	packword12(1,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,1,0,0,1,0,0,0,1,0,0,0),
	packword12(0,0,0,0,0,0,0,0,0,0,1,1),
	packword12(1,1,0,0,0,0,0,0,0,0,0,0),
	packword12(0,0,0,1,0,0,0,1,0,0,1,0),
	packword12(0,0,1,0,0,0,0,0,1,0,0,1),
	packword12(0,1,0,0,0,1,0,0,1,0,0,0),
	packword12(0,0,0,0,0,1,0,0,0,0,0,0)
};

const int alien_top_in_12x8[] =
{
	packword12(0,0,0,0,0,1,1,0,0,0,0,0),
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(0,0,1,1,1,1,1,1,1,1,0,0),
	packword12(0,0,0,1,0,1,1,0,1,0,0,0),
	packword12(0,0,1,0,0,0,0,0,0,1,0,0),
	packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};

const int alien_top_out_12x8[] =
{
	packword12(0,0,0,0,0,1,1,0,0,0,0,0),
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(0,0,1,1,1,1,1,1,1,1,0,0),
	packword12(0,0,0,0,1,0,0,1,0,0,0,0),
	packword12(0,0,0,1,0,1,1,0,1,0,0,0),
	packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

const int alien_middle_in_12x8[] =
{
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,0,0,0,1,0,0,0,1,0,0,0),
	packword12(0,0,0,1,1,1,1,1,1,1,0,0),
	packword12(0,0,1,1,0,1,1,1,0,1,1,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,1,0,1,0,0,0,0,0,1,0,1),
	packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};

const int alien_middle_out_12x8[] =
{
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,1,0,0,1,0,0,0,1,0,0,1),
	packword12(0,1,0,1,1,1,1,1,1,1,0,1),
	packword12(0,1,1,1,0,1,1,1,0,1,1,1),
	packword12(0,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,1,1,1,1,1,1,1,1,1,0),
	packword12(0,0,0,1,0,0,0,0,0,1,0,0),
	packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};

const int alien_bottom_in_12x8[] =
{
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,0),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(1,1,1,0,0,1,1,0,0,1,1,1),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,1,1,1,0,0,1,1,1,0,0),
	packword12(0,1,1,0,0,1,1,0,0,1,1,0),
	packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};

const int alien_bottom_out_12x8[] =
{
	packword12(0,0,0,0,1,1,1,1,0,0,0,0),
	packword12(0,1,1,1,1,1,1,1,1,1,1,0),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(1,1,1,0,0,1,1,0,0,1,1,1),
	packword12(1,1,1,1,1,1,1,1,1,1,1,1),
	packword12(0,0,0,1,1,0,0,1,1,0,0,0),
	packword12(0,0,1,1,0,1,1,0,1,1,0,0),
	packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

const int tank_15x8[] =
{
	packword15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};


// Shape of the entire bunker.
const int bunker_24x18[] =
{
	packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
	packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
const int bunkerDamage0_6x6[] = {
	packword6(0,1,1,0,0,0),
	packword6(0,0,0,0,0,1),
	packword6(1,1,0,1,0,0),
	packword6(1,0,0,0,0,0),
	packword6(0,0,1,1,0,0),
	packword6(0,0,0,0,1,0)
};

const int bunkerDamage1_6x6[] = {
	packword6(1,1,1,0,1,0),
	packword6(1,0,1,0,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,0,0,0,0,0),
	packword6(0,1,1,1,0,1),
	packword6(0,1,1,0,1,0)
};

const int bunkerDamage2_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,0,1,1,0,1),
	packword6(1,1,0,1,1,1),
	packword6(1,1,0,1,1,0),
	packword6(0,1,1,1,0,1),
	packword6(1,1,1,1,1,1)
};

const int bunkerDamage3_6x6[] = {
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1),
	packword6(1,1,1,1,1,1)
};

Sprite initSprite(int height, int width, uint32_t color, const int *sprite){
	Sprite sp;
	sp.Color.color = color;
	sp.height = height;
	sp.width = width;
	sp.sprite = sprite;
	return sp;
}

Position initPosition(int x, int y){
	Position p;
	p.x = x;
	p.y = y;
	return p;
}

Tank initTank(int x, int y){
	Tank t;
	t.lives = 3;
	t.p = initPosition(x, y);
	t.sp = initSprite(TANK_HEIGHT, TANK_WIDTH, GREEN, tank_15x8);
	return t;
}

Bunker initBunker(int x, int y){
	Bunker b;
	b.erosionLevel = 0;
	b.p = initPosition(x, y);
	b.sp = initSprite(BUNKER_HEIGHT, BUNKER_WIDTH, GREEN, bunker_24x18);
}

Alien initAlien(int x, int y){
	Alien a;
	a.alive = 1;
	a.p = initPosition(x, y);
}
