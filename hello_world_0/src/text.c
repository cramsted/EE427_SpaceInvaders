/*
 * text.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "text.h"
#include "render.h"
extern const int s_8x5[];
extern const int c_8x5[];
extern const int o_8x5[];
extern const int r_8x5[];
extern const int e_8x5[];
extern const int l_8x5[];
extern const int i_8x5[];
extern const int v_8x5[];
extern const int zero_8x5[];
extern const int one_8x5[];
extern const int two_8x5[];
extern const int three_8x5[];
extern const int four_8x5[];
extern const int five_8x5[];
extern const int six_8x5[];
extern const int seven_8x5[];
extern const int eight_8x5[];
extern const int nine_8x5[];

void drawCharacters() {
	const int *scoreArray[] = { s_8x5, c_8x5, o_8x5, r_8x5, e_8x5 };
	int i;
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
		Character s = initChar(
				(SCORE_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, scoreArray[i]);
		edit_frameBuffer(&s.sp, &s.p);
	}

	const int *livesArray[] = { l_8x5, i_8x5, v_8x5, e_8x5, s_8x5 };
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
		Character s = initChar(
				(LIVES_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, livesArray[i]);
		edit_frameBuffer(&s.sp, &s.p);
	}
}

Character initChar(int x, int y, const int *sprite) {
	Character c;
	c.p.x = x;
	c.p.y = y;
	c.sp = initSprite(CHARACTER_HEIGHT, CHARACTER_WIDTH, WHITE, sprite);
	return c;
}
