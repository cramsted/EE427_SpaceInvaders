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

void readInput() {
	char c, c2;
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
//		xil_printf("Which alien do you want to kill (format XX)?: ");
//
//		fgets(str,2,stdin);
//		xil_printf("%s\n\r", str);

//		c = getchar();
//		if (!isDigit(c)) {
//			xil_printf("Invalid digit\n\r");
//			return;
//		}
//		xil_printf("%c", c);
//		c2 = getchar();
//		if (!isDigit(c2)) {
//			c2 = '0';
//		}
//		xil_printf("%c\n\r", c);
//		c -= '0';
//		c2 -= '0';
//		int c3 = (int) ((c * 10) + c2);
//		killAlien(&aliens, c3);
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
		erodeBunker(0, 0, 0);
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
