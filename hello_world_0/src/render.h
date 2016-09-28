/*
 * render.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "sprites.h"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640

void init_videoDMAController();
void init();
void render();
void edit_frameBuffer(Sprite *sp, Position *p);

#endif /* RENDER_H_ */

