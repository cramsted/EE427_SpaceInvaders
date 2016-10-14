/*
 * sprites.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "sprites.h"

//initializes a sprite
Sprite initSprite(uint32_t height, uint32_t width, uint32_t color, const uint32_t *sprite) {
	Sprite sp;
	sp.Color.color = color;
	sp.height = height;
	sp.width = width;
	sp.sprite = sprite;
	return sp;
}

//initializes and x,y coordinate
Position initPosition(uint32_t x, uint32_t y) {
	Position p;
	p.x = x;
	p.y = y;
	return p;
}
