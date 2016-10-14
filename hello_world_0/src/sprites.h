/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h> //used for uint8_t

//y coordinate of the ground
#define GROUND_START_Y 450

//colors
#define GREEN 	0x0000FF00
#define WHITE 	0x00FFFFFF
#define BLACK 	0x00000000
#define RED		0x00FF0000

//meta data for the sprites
typedef struct {
	uint32_t height;
	uint32_t width;
	union { //used to make a single 32 bit int. Works well with our #defined colors
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t padding;
		} RGB;
		uint32_t color;
	} Color;
	const uint32_t *sprite; //points to the actual sprite struct
} Sprite;

//struct for sprite positional data
typedef struct {
	uint32_t x;
	uint32_t y;
} Position;

//initializes a sprite
Sprite initSprite(uint32_t height, uint32_t width, uint32_t color, const uint32_t *sprite);
//initializes and x,y coordinate
Position initPosition(uint32_t x, uint32_t y);

#endif /* SPRITES_H_ */
