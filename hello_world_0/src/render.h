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
void initVideoDMAController();
//initializes the screen for a game
void videoInit();
//switches between frame buffers
void render();
//for drawing sprites to the screen
void editFrameBuffer(Sprite *sp, Position *p);
//changes which frame is in use.
void changeFrame(uint8_t x);
//copies everything in framebuffer0 to framebuffer1
void screenShot();
#endif /* RENDER_H_ */
