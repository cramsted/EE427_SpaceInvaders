/*
 * text.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "text.h"
#include "render.h"	//needed for edit_frameBuffer
#include <stdio.h> //for debugging
extern const int s_8x5[];
extern const int c_8x5[];
extern const int o_8x5[];
extern const int r_8x5[];
extern const int e_8x5[];
extern const int l_8x5[];
extern const int i_8x5[];
extern const int v_8x5[];
extern const int g_8x5[];
extern const int a_8x5[];
extern const int m_8x5[];
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

Score score;

//funciton prototypes
void initDigitSprites();
void drawScore();

const int *digitsArray[] = { zero_8x5, one_8x5, two_8x5, three_8x5, four_8x5,
		five_8x5, six_8x5, seven_8x5, eight_8x5, nine_8x5 };

void drawCharacters() {
	const int *scoreArray[] = { s_8x5, c_8x5, o_8x5, r_8x5, e_8x5 };
	int i;
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
		Character s = initChar(
				(SCORE_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, scoreArray[i], WHITE);
		editFrameBuffer(&s.sp, &s.p);
	}

	const int *livesArray[] = { l_8x5, i_8x5, v_8x5, e_8x5, s_8x5 };
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
		Character s = initChar(
				(LIVES_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, livesArray[i], WHITE);
		editFrameBuffer(&s.sp, &s.p);
	}
}

void drawGameOver() {
	const int *gameArray[] = { g_8x5, a_8x5, m_8x5, e_8x5 };
	int i;
	for (i = 0; i < MAX_CHARACTER_LETTERS - 1; i++) {
		Character s = initChar(
				(GAME_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				GAME_OVER_Y, gameArray[i], RED);
		editFrameBuffer(&s.sp, &s.p);
	}
	const int *overArray[] = { o_8x5, v_8x5, e_8x5, r_8x5 };
	for (i = 0; i < MAX_CHARACTER_LETTERS - 1; i++) {
		Character s = initChar(
				(OVER_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				GAME_OVER_Y, overArray[i], RED);
		editFrameBuffer(&s.sp, &s.p);
	}
}

Character initChar(int x, int y, const int *sprite, uint32_t color) {
	Character c;
	c.p.x = x;
	c.p.y = y;
	c.sp = initSprite(CHARACTER_HEIGHT, CHARACTER_WIDTH, color, sprite);
	return c;
}

void initScore() {
	score.score = 0;
	int i;
	for (i = 0; i < MAX_SCORE_DIGITS; i++) {
		score.sp[i] = initChar(
				DIGITS_START_X + (i * (CHARACTER_WIDTH + CHARACTER_PADDING)),
				TEXT_START_Y, digitsArray[0], RED);
	}
	drawScore();
}

void drawScore() {
	int digit_one = (score.score / 1000) % 10;
	int digit_two = (score.score / 100) % 10;
	int digit_three = (score.score / 10) % 10;
	int digit_four = 0;

	score.sp[0].sp.sprite = digitsArray[digit_one];
	score.sp[1].sp.sprite = digitsArray[digit_two];
	score.sp[2].sp.sprite = digitsArray[digit_three];
	score.sp[3].sp.sprite = digitsArray[digit_four];
	int i;
	for (i = 0; i < MAX_SCORE_DIGITS; i++) {
		editFrameBuffer(&score.sp[i].sp, &score.sp[i].p);
	}
}

void drawUfoPoints(Position *p, int points, uint32_t color) {
	int digit_one = (points / 100) % 10;
	int digit_two = (points / 10) % 10;
	int digit_three = 0;

	Character hundreds = initChar(p->x, p->y, digitsArray[digit_one], color);
	editFrameBuffer(&hundreds.sp, &hundreds.p);
	Character tens = initChar((p->x + CHARACTER_WIDTH + CHARACTER_PADDING),
			p->y, digitsArray[digit_two], color);
	editFrameBuffer(&tens.sp, &tens.p);
	Character ones = initChar((p->x + (CHARACTER_WIDTH + CHARACTER_PADDING) * 2), p->y, digitsArray[digit_three], color);
	editFrameBuffer(&ones.sp, &ones.p);
}

void updateScore(uint32_t points) {
	score.score += points;
	drawScore();
}
