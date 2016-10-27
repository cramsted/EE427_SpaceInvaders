/*
 * sprite_char_bit_maps.c
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#include <stdint.h>

 //macros for the sprite pixel arrays
#define packword8(b7,b6,b5,b4,b3,b2,b1,b0) \
((b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

//letter s
const uint32_t s_8x5[] =
{
		packword8(0,0,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0)
};

//letter c
const uint32_t c_8x5[] =
{
		packword8(0,0,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,1,1)
};

//letter o
const uint32_t o_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

//letter r
const uint32_t r_8x5[] =
{
		packword8(1,1,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1)
};

//letter e
const uint32_t e_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

//letter l
const uint32_t l_8x5[] =
{
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

//letter i
const uint32_t i_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

//letter v
const uint32_t v_8x5[] =
{
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,1,1,0,0,1,1,0),
		packword8(0,0,0,1,1,0,0,0)
};

//letter g
const uint32_t g_8x5[] =
{
		packword8(0,0,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

//letter a
const uint32_t a_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1)
};

//letter m
const uint32_t m_8x5[] =
{
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,0,0,1,1,1),
		packword8(1,1,0,1,1,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1)
};

//number 0
const uint32_t zero_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,1,1,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

//number 1
const uint32_t one_8x5[] =
{
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,1,1,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

//number 2
const uint32_t two_8x5[] =
{
		packword8(1,1,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,1,1)
};

//number 3
const uint32_t three_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1)
};

//number4
const uint32_t four_8x5[] =
{
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1)
};

//number 5
const uint32_t five_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0)
};

//number 6
const uint32_t six_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1)
};

//number 7
const uint32_t seven_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1)
};

//number 8
const uint32_t eight_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

//number 9
const uint32_t nine_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1)
};
