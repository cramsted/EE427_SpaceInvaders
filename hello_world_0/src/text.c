/*
 * text.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "text.h"
#include "render.h"	//needed for edit_frameBuffer
#include <stdio.h> //for debugging

// Constants
#define THOUSANDS 1000
#define HUNDREDS 100
#define TENS 10
#define TEN 10

// Macros
#define CHARACTER_OFFSET (CHARACTER_WIDTH + CHARACTER_PADDING)

// We use the following sprites for text:
// They're all letters and numbers
extern const uint32_t s_8x5[];
extern const uint32_t c_8x5[];
extern const uint32_t o_8x5[];
extern const uint32_t r_8x5[];
extern const uint32_t e_8x5[];
extern const uint32_t l_8x5[];
extern const uint32_t i_8x5[];
extern const uint32_t v_8x5[];
extern const uint32_t g_8x5[];
extern const uint32_t a_8x5[];
extern const uint32_t m_8x5[];
extern const uint32_t zero_8x5[];
extern const uint32_t one_8x5[];
extern const uint32_t two_8x5[];
extern const uint32_t three_8x5[];
extern const uint32_t four_8x5[];
extern const uint32_t five_8x5[];
extern const uint32_t six_8x5[];
extern const uint32_t seven_8x5[];
extern const uint32_t eight_8x5[];
extern const uint32_t nine_8x5[];

// The score object keeps track of the current game score and is
// used when drawing the score on the screen
Score score;

//function prototypes:
void drawScore();

// It's convenient to have an array of pointers to the digit sprites,
// because we can index into the array to get the sprite to draw
const uint32_t *digitsArray[] = { zero_8x5, one_8x5, two_8x5, three_8x5, four_8x5,
		five_8x5, six_8x5, seven_8x5, eight_8x5, nine_8x5 };

// Draws the initial text on the screen (the words "SCORE" and "LIVES")
// This is called once at the beginning of the game
void drawCharacters() {
    // We create an array of pointers to the letters so we can index this
    // array in a loop as we draw the letters on the screen
    // This draws the word "SCORE"
	const uint32_t *scoreArray[] = { s_8x5, c_8x5, o_8x5, r_8x5, e_8x5 };
	uint32_t i;
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
        // Initialize and draw the current letter on the screen
        // We do a little bit of simple math to calculate the starting
        // x coordinate for each successive letter
		Character s = initChar(
				(SCORE_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, scoreArray[i], WHITE);
		editFrameBuffer(&s.sp, &s.p);
	}

    // Drawing "LIVES" on the screen is the same process as drawing "SCORE"
	const uint32_t *livesArray[] = { l_8x5, i_8x5, v_8x5, e_8x5, s_8x5 };
	for (i = 0; i < MAX_CHARACTER_LETTERS; i++) {
		Character s = initChar(
				(LIVES_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				TEXT_START_Y, livesArray[i], WHITE);
		editFrameBuffer(&s.sp, &s.p);
	}
}

// Called when the game ends
// This draws the words "GAME OVER" on the screen in red letters
// and uses the same process as drawing "SCORE" and "LIVES"
// We should probably have combined this duplicate code into a single
// function, but we ran out of time to refactor and beautify our code.
void drawGameOver() {
	const uint32_t *gameArray[] = { g_8x5, a_8x5, m_8x5, e_8x5 };
	uint32_t i;
	for (i = 0; i < MAX_CHARACTER_LETTERS - 1; i++) {
		Character s = initChar(
				(GAME_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				GAME_OVER_Y, gameArray[i], RED);
		editFrameBuffer(&s.sp, &s.p);
	}
	const uint32_t *overArray[] = { o_8x5, v_8x5, e_8x5, r_8x5 };
	for (i = 0; i < MAX_CHARACTER_LETTERS - 1; i++) {
		Character s = initChar(
				(OVER_START_X + (CHARACTER_WIDTH + CHARACTER_PADDING) * i),
				GAME_OVER_Y, overArray[i], RED);
		editFrameBuffer(&s.sp, &s.p);
	}
}

// Initializes and returns a Character object at the coordinates (x,y)
// with the chosen sprite and color
Character initChar(uint32_t x, uint32_t y, const uint32_t *sprite, uint32_t color) {
	Character c;
	c.p.x = x;
	c.p.y = y;
	c.sp = initSprite(CHARACTER_HEIGHT, CHARACTER_WIDTH, color, sprite);
	return c;
}

// Initializes the Score object and draws the score on the screen
// The score object is actually the numbers, not the text "SCORE"
// It consists of four digits, since in our game we figured players
// won't score over 9990
void initScore() {
	score.score = 0;
	uint32_t i;
	for (i = 0; i < MAX_SCORE_DIGITS; i++) {
		score.sp[i] = initChar(
				DIGITS_START_X + (i * (CHARACTER_WIDTH + CHARACTER_PADDING)),
				TEXT_START_Y, digitsArray[0], RED);
	}
	drawScore();
}

// Draws the score on the screen.
// It divides and mods the thousands, hundreds, and tens places
// to get the correct digit in each place
// Because the score is always a multiple of 10,
// the last digit is always a zero.
void drawScore() {
	uint32_t digit_one = (score.score / THOUSANDS) % TEN;
	uint32_t digit_two = (score.score / HUNDREDS) % TEN;
	uint32_t digit_three = (score.score / TENS) % TEN;
	uint32_t digit_four = 0;

    // We can easily change the sprites by changing the pointer:
    // Each digit points to the sprite of the number it represents
	score.sp[0].sp.sprite = digitsArray[digit_one];
	score.sp[1].sp.sprite = digitsArray[digit_two];
	score.sp[2].sp.sprite = digitsArray[digit_three];
	score.sp[3].sp.sprite = digitsArray[digit_four];
    
    // Loop through the digits to draw them on the screen
	uint32_t i;
	for (i = 0; i < MAX_SCORE_DIGITS; i++) {
		editFrameBuffer(&score.sp[i].sp, &score.sp[i].p);
	}
}

// Draws the points awarded to the player for killing the UFO on the screen
// It's always a multiple of 10, so the last digit is zero
// We need to draw it on the screen where the UFO was killed, so that
// position is passed to this function, as well as the points
// awarded and the color to draw the numbers
// We can easily calculate the x position by adding an offset
// of the character width and mulitplying it by how many characters
// over it is.
void drawUfoPoints(Position *p, uint32_t points, uint32_t color) {
	uint32_t digit_one = (points / HUNDREDS) % TEN;
	uint32_t digit_two = (points / TENS) % TEN;
	uint32_t digit_three = 0;

    // The hundreds place has no offset, then the tens has
    // an offset of one character, and the ones place has an offset of
    // two characters
	Character hundreds = initChar(p->x, p->y, digitsArray[digit_one], color);
	editFrameBuffer(&hundreds.sp, &hundreds.p);
    
	Character tens = initChar((p->x + CHARACTER_OFFSET),
			p->y, digitsArray[digit_two], color);
	editFrameBuffer(&tens.sp, &tens.p);
    
	Character ones = initChar((p->x + (CHARACTER_OFFSET) * 2), p->y, digitsArray[digit_three], color);
	editFrameBuffer(&ones.sp, &ones.p);
}

// Updates the score and draws the new score on the screen
void updateScore(uint32_t points) {
	score.score += points;
	drawScore();
}
