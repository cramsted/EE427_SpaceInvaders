/*
 * sprite_char_bit_maps.c
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#define packword8(b7,b6,b5,b4,b3,b2,b1,b0) \
((b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

const int s_8x5[] =
{
		packword8(0,0,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0)
};

const int c_8x5[] =
{
		packword8(0,0,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,1,1)
};

const int o_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

const int r_8x5[] =
{
		packword8(1,1,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1)
};

const int e_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

const int l_8x5[] =
{
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

const int i_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

const int v_8x5[] =
{
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,1,1,0,0,1,1,0),
		packword8(0,0,0,1,1,0,0,0)
};

const int zero_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,1,1,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

const int one_8x5[] =
{
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,1,1,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(0,0,0,1,1,0,0,0),
		packword8(1,1,1,1,1,1,1,1)
};

const int two_8x5[] =
{
		packword8(1,1,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,1,1)
};

const int three_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1)
};

const int four_8x5[] =
{
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1)
};

const int five_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(0,0,1,1,1,1,0,0),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,0,0)
};

const int six_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,0,0),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1)
};

const int seven_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(0,0,0,0,0,0,1,1)
};

const int eight_8x5[] =
{
		packword8(0,0,1,1,1,1,0,0),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(0,0,1,1,1,1,0,0)
};

const int nine_8x5[] =
{
		packword8(1,1,1,1,1,1,1,1),
		packword8(1,1,0,0,0,0,1,1),
		packword8(1,1,0,1,1,0,1,1),
		packword8(0,0,0,0,0,0,1,1),
		packword8(1,1,1,1,1,1,1,1)
};
