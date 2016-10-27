/*
 * sprite_bit_maps.c
 *
 *  Created on: Sep 24, 2016
 *      Author: superman
 */

#include <stdint.h>

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

#define packword3(b2,b1,b0) \
((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

const uint32_t saucer_16x7[] =
{
	packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
	packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
	packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
	packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
	packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
	packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
	packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

const uint32_t ufo_explosion_16x7[] =
{
	packword16(0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0),
	packword16(0,0,0,0,1,0,0,0,1,1,1,0,1,0,0,0),
	packword16(0,0,0,0,1,1,0,1,0,1,1,0,0,1,0,0),
	packword16(0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0),
	packword16(1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,0),
	packword16(0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0),
	packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

const uint32_t alien_explosion_12x10[] =
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

const uint32_t alien_top_in_12x8[] =
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

const uint32_t alien_top_out_12x8[] =
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

const uint32_t alien_middle_in_12x8[] =
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

const uint32_t alien_middle_out_12x8[] =
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

const uint32_t alien_bottom_in_12x8[] =
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

const uint32_t alien_bottom_out_12x8[] =
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

const uint32_t alien_explosion_12x8[] =
{
	packword12(0,0,0,0,1,1,0,0,0,0,0,0),
	packword12(0,1,1,1,1,0,1,1,0,1,1,0),
	packword12(0,0,0,1,0,1,0,1,0,1,1,0),
	packword12(0,1,0,0,1,1,0,0,0,1,0,0),
	packword12(0,1,1,1,1,0,1,1,0,1,1,0),
	packword12(0,0,0,1,0,1,0,1,0,1,1,0),
	packword12(0,1,0,0,1,1,0,0,0,0,0,0),
	packword12(1,0,0,1,0,1,0,1,0,1,1,0)
};

const uint32_t tank_15x8[] =
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

//tank explostion sprite
const uint32_t tank_explosion_15x8[] =
{
	packword15(0,1,0,0,0,0,1,0,0,0,1,0,0,1,0),
	packword15(0,0,0,1,1,0,0,0,1,0,1,0,0,0,0),
	packword15(1,0,0,1,0,0,0,0,0,1,0,0,0,1,0),
	packword15(0,1,0,0,0,0,1,0,0,0,1,0,0,1,0),
	packword15(0,0,0,1,1,0,0,0,1,0,1,0,0,0,0),
	packword15(1,0,0,1,0,0,0,0,0,1,0,0,0,1,0),
	packword15(0,1,0,0,0,0,1,0,0,0,1,0,0,1,0),
	packword15(1,0,0,1,0,0,0,0,0,1,0,0,0,1,0)
};

const uint32_t tank_explosion2[] =
{
	packword15(1,0,1,0,1,0,1,0,0,1,0,1,0,0,1),
	packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
	packword15(1,0,0,1,1,0,0,0,0,0,1,0,1,0,1),
	packword15(0,0,1,1,1,0,1,1,1,1,1,0,0,1,0),
	packword15(1,0,1,0,0,1,0,0,0,0,1,0,0,0,1),
	packword15(0,0,1,1,1,1,0,0,1,0,1,1,1,0,1),
	packword15(1,1,0,1,0,1,1,0,1,0,0,0,1,1,1),
	packword15(1,0,0,0,0,1,1,1,1,1,1,1,0,0,0)
};

// Shape of the entire bunker.
const uint32_t bunker_24x18[] =
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
const uint32_t bunkerDamage0_6x6[] = {
	packword6(1,0,0,1,1,1),
	packword6(1,1,1,1,1,0),
	packword6(0,0,1,0,1,1),
	packword6(0,1,1,1,1,1),
	packword6(1,1,0,0,1,1),
	packword6(1,1,1,1,0,1)
};

const uint32_t bunkerDamage1_6x6[] = {
	packword6(0,0,0,1,0,1),
	packword6(0,1,0,1,1,0),
	packword6(0,0,1,0,0,0),
	packword6(0,1,1,1,1,1),
	packword6(1,0,0,0,1,0),
	packword6(1,0,0,1,0,1)
};

const uint32_t bunkerDamage2_6x6[] = {
	packword6(0,0,0,0,0,0),
	packword6(0,1,0,0,1,0),
	packword6(0,0,1,0,0,0),
	packword6(0,0,1,0,0,1),
	packword6(1,0,0,0,1,0),
	packword6(0,0,0,0,0,0)
};

const uint32_t bunkerDamage3_6x6[] = {
	packword6(0,0,0,0,0,0),
	packword6(0,0,0,0,0,0),
	packword6(0,0,0,0,0,0),
	packword6(0,0,0,0,0,0),
	packword6(0,0,0,0,0,0),
	packword6(0,0,0,0,0,0)
};

const uint32_t bulletCross_3x5[] = {
	packword3(0,1,0),
	packword3(0,1,0),
	packword3(1,1,1),
	packword3(0,1,0),
	packword3(0,1,0)
};

const uint32_t bulletLightning_3x5[] = {
	packword3(0,1,0),
	packword3(1,0,0),
	packword3(0,1,0),
	packword3(0,0,1),
	packword3(0,1,0)
};
