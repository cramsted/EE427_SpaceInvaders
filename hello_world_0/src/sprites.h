/*
 * sprites.h
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <stdint.h>

#define CHARACTER_HEIGHT (5 * 2)
#define CHARACTER_WIDTH (8 * 2)


#define MAX_LIVES 3

#define MAX_SCORE_DIGITS 4
#define MAX_CHARACTER_LETTERS 5







#define SCORE_START_X 20
#define LIVES_START_X 350
#define TEXT_START_Y 25
#define XLIFE_PADDING 10
#define CHARACTER_PADDING 4
#define GROUND_START_Y 450

//colors
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define BLACK 0x00000000

//meta data for the sprites
typedef struct {
	int height;
	int width;
	union {
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t padding;
		} RGB;
		uint32_t color;
	} Color;
	const int *sprite;
} Sprite;

typedef struct {
	int x;
	int y;
} Position;









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
Sprite initSprite(int height, int width, uint32_t color, const int *sprite);
Position initPosition(int x, int y);
#endif /* SPRITES_H_ */
