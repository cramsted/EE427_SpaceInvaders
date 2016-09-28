/*
 * controls.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "controls.h"
#include <stdio.h>

void readInput(){
	char c;
	xil_printf("enter: ");
	c = getchar();

	xil_printf("you entered %c\n\r", c);

	if (c < '0' || c > '9') {
		xil_printf("Invalid digit\n\r");
		return;
	}

	// turn it into a digit
	c -= '0';

	switch (c) {
	case 2: // kill alien
		break;
	case 3: // fire random missile
		break;
	case 4: // move tank left
		break;
	case 5: // fire tank bullet
		break;
	case 6: // move tank right
		break;
	case 7: // erode bunker
		break;
	case 8: // update alien position
		break;
	case 9: // update all bullets
		break;
	default:
		return;
	}
}
