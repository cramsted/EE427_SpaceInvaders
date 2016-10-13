/*
 * ufo.h
 *
 *  Created on: Oct 10, 2016
 *      Author: superman
 */

#ifndef UFO_H_
#define UFO_H_

#include "sprites.h"

#define UFO_WIDTH (16 * 2)
#define UFO_HEIGHT (7 * 2)
#define UFO_SHIFT_X 4
#define UFO_PADDING 8
#define UFO_START_X_LEFT UFO_PADDING
#define UFO_START_X_RIGHT (640-UFO_PADDING-UFO_WIDTH)
#define UFO_START_Y 50

typedef enum {
	inactive,
	active
} ufo_status_e;

typedef enum {
	ufo_left = -UFO_SHIFT_X,
	ufo_right = UFO_SHIFT_X
}ufo_direction_e;

typedef struct {
	ufo_direction_e direction; //what direction the ufo is currently traveling in
	ufo_status_e status;
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
void ufoAppear();
void ufoDisapear();

#endif /* UFO_H_ */
