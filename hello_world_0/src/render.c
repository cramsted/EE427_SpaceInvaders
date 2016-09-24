/*
 * render.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "render.h"
#include "sprites.h"

void init() {
	// TODO: initialize various structs, etc.
	// init alien positions and draw them
	// init tank position and lives and draw it
	// init bunker positions and erosion and draw them
	// init score
}

int findPixelValue(int x, int y, int col, int row, int *sprite){
	int xval = (col - x) >> 1;
	int yval = (row - y) >> 1;
	return (sprite[yval] << xval) & 0x80000000;
}
