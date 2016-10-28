/*
 * ufo.h
 *
 *  Created on: Oct 10, 2016
 *      Author: superman
 */

#ifndef UFO_H_
#define UFO_H_

#include "sprites.h"

// CONSTANTS - all sizes are in pixels
#define UFO_WIDTH (16 * 2)
#define UFO_HEIGHT (7 * 2)
#define UFO_SHIFT_X 4
#define UFO_PADDING 8
#define UFO_START_X_LEFT UFO_PADDING
#define UFO_START_X_RIGHT (640-UFO_PADDING-UFO_WIDTH)
#define UFO_START_Y 50

// So we can tell if the UFO is on the screen or not.
typedef enum {
	inactive,
	active
} ufo_status_e;

// We just use these when we want to move the UFO, e.g. ufo.p.x += ufo_left or ufo_right
typedef enum {
	ufo_left = -UFO_SHIFT_X,
	ufo_right = UFO_SHIFT_X
}ufo_direction_e;

// Describes a UFO objet
typedef struct {
	ufo_direction_e direction; //what direction the ufo is currently traveling in
	ufo_status_e status;
	Position p; //x,y position of the sprite
	Sprite sp; //sprite object
}UFO;

// Give others access to the UFO object
extern UFO ufo;

// basic initialization of UFO object, including sprite and position
void initUfo();

// moves the ufo in the direction of ufo_direction_e by a preset number of pixels
void updateUfo();

// kills the ufo
void killUfo();

// Draw it on the screen
void drawUfo();

// Erase the score
void eraseUfoExplosionSprite();

// Called when the UFO first comes on the screen
void ufoAppear();

// Called when the UFO exits the screen
void ufoDisapear();

#endif /* UFO_H_ */
