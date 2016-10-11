/*
 * ufo.h
 *
 *  Created on: Oct 10, 2016
 *      Author: superman
 */

#ifndef UFO_H_
#define UFO_H_

#include "sprites.h"

#define UFO_SHIFT_X 5
#define UFO_START_X 8
#define UFO_START_Y 50
#define UFO_WIDTH (16 * 2)
#define UFO_HEIGHT (7 * 2)
typedef enum {
	left = -UFO_SHIFT_X,
	right = UFO_SHIFT_X
}ufo_direction_e;

typedef struct {
	ufo_direction_e direction; //what direction the ufo is currently traveling in
	Position p; //x,y position of the sprite
	Sprite sp; //sprite object
}UFO;

extern UFO ufo;

void initUfo();
//moves the ufo in the direction of ufo_direction_e by a preset number of pixels
void updateUfo();
//kills the ufo
void killUfo();
void drawUfo();
void eraseUfoExplosionSprite();

#endif /* UFO_H_ */
