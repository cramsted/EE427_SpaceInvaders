/*
 * ufo.c
 *
 *  Created on: Oct 10, 2016
 *      Author: superman
 */

#include "ufo.h"

UFO ufo;
extern const int saucer_16x7[];

void initUfo(){
	ufo.p = initPosition(UFO_START_X, UFO_START_Y);
	ufo.sp = initSprite(UFO_HEIGHT, UFO_WIDTH, RED, saucer_16x7);
	ufo.direction = right;
}

//moves the ufo in the direction of ufo_direction_e by a preset number of pixels
void updateUfo(){

}
//kills the ufo
void killUfo();
void drawUfo();
void eraseUfoExplosionSprite();
