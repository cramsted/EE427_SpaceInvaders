/*
 * capsense.c
 *
 *  Created on: Nov 17, 2016
 *      Author: superman
 */

#include "capsense.h"
#include "xparameters.h"
#include "xil_io.h"

uint32_t capsenseRead() {
	return Xil_In32(XPAR_CAPACITIVE_BUTTONS_0_BASEADDR);
}
