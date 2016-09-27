/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h>

#define GROUND_START_Y 450

//colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define BLACK 0x00000000

//meta data for the sprites
typedef struct {
	int height;
	int width;
	union {
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t padding;
		} RGB;
		uint32_t color;
	} Color;
	const int *sprite;
} Sprite;

typedef struct {
	int x;
	int y;
} Position;

Sprite initSprite(int height, int width, uint32_t color, const int *sprite);
Position initPosition(int x, int y);
#endif /* SPRITES_H_ */
