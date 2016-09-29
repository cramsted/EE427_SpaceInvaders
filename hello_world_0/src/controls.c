/*
 * controls.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "controls.h"
#include "globals.h"
#include <stdio.h>

int isDigit(char c);
int getAlienNumber();

void readInput() {
	char c;
	int num;
	//	xil_printf("enter: ");
	c = getchar();

	//	xil_printf("you entered %c\n\r", c);

	if (!isDigit(c)) {
		xil_printf("Invalid digit\n\r");
		return;
	}

	// turn it into a digit
	c -= '0';

	//	char str[2];

	switch (c) {
	case 2: // kill alien
		num = getAlienNumber();
		if (num >= 0 && num <= 54) {
			xil_printf("kill alien %d\n\r", num);
			// convert 0-54 to row/col
			int row = num / ALIENS_COL;
			int col = num % ALIENS_COL;
			killAlien(&aliens, row, col);
		} else {
			xil_printf("invalid input; enter a new command\n\r");
		}
		break;
	case 3: // fire random missile
		alienPew(&aliens, &bullets);
		break;
	case 4: // move tank left
		moveTankLeft(&tank);
		break;
	case 5: // fire tank bullet
		tankPew(&tank, &bullets);
		break;
	case 6: // move tank right
		moveTankRight(&tank);
		break;
	case 7: // erode bunker
		num = getAlienNumber();
		if (num >= 0 && num < MAX_BUNKERS) {
			erodeWholeBunker(num);
		}
		break;
	case 8: // update alien position
		updateAliens(&aliens);
		break;
	case 9: // update all bullets
		updateBullets(&bullets);
		break;
	default:
		return;
	}
}

int isDigit(char c) {
	if (c < '0' || c > '9') {
		return 0;
	}
	return 1;
}

int getAlienNumber() {
	xil_printf("which alien number would you like to kill?\n\r");
	char d10pos = getchar();
	char d1pos = getchar();
	if (!isDigit(d10pos))
		return -1;
	if (!isDigit(d1pos)) {
		return d10pos - '0';
	} else {
		d10pos -= '0';
		d1pos -= '0';
		return (d10pos * 10) + d1pos;
	}
}
