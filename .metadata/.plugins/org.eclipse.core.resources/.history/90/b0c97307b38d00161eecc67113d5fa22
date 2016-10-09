/*
 * render.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "sprites.h"

//values for the dimensions of the screen
#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640

//sets up the video DMA controller
void init_videoDMAController();
//initializes the screen for a game
void videoInit();
//switches between frame buffers
void render();
//for drawing sprites to the screen
void edit_frameBuffer(Sprite *sp, Position *p);

#endif /* RENDER_H_ */
