/*
 * sprites.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "sprites.h"
#include <stdio.h>
#include <string.h>

//Sprite dimensions













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

Character initChar(int x, int y, const int *sprite) {
	Character c;
	c.p.x = x;
	c.p.y = y;
	c.sp = initSprite(CHARACTER_HEIGHT, CHARACTER_WIDTH, WHITE, sprite);
	return c;
}


