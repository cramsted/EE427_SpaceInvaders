/*
 * text.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "sprites.h"

#define MAX_SCORE_DIGITS 4
#define MAX_CHARACTER_LETTERS 5
#define SCORE_START_X 20
#define LIVES_START_X 350
#define TEXT_START_Y 25
#define CHARACTER_PADDING 4
#define CHARACTER_HEIGHT (5 * 2)
#define CHARACTER_WIDTH (8 * 2)

typedef struct {
	Position p;
	uint32_t score;
	Sprite *sp[MAX_SCORE_DIGITS];
} Score;

typedef struct {
	Position p;
	Sprite sp;
} Character;

Character initChar(int x, int y, const int *sprite);
void drawCharacters();

#endif /* TEXT_H_ */
