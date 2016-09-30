/*
 * sprites.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "sprites.h"

//initializes a sprite
Sprite initSprite(int height, int width, uint32_t color, const int *sprite) {
	Sprite sp;
	sp.Color.color = color;
	sp.height = height;
	sp.width = width;
	sp.sprite = sprite;
	return sp;
}

//initializes and x,y coordinate
Position initPosition(int x, int y) {
	Position p;
	p.x = x;
	p.y = y;
	return p;
}
