/*
 * render.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 *
 * TODO's:
 *   randomize alien bullets
 *   kill aliens (take input)
 *   bullets - destroy when reach limits (off screen or ground)
 *   erode bunkers (take input)
 */
#include "render.h"
#include <stdio.h>
#include "sprites.h"
#include "xaxivdma.h"
#include "xparameters.h"
#include "xio.h"
#include "globals.h"

//function prototypes
int findPixelValue(int x, int y, int col, int row, Sprite *sp);
void drawGround();

// Starting location in DDR where we will store the images that we display.
#define FRAME_BUFFER_0_ADDR 0xC1000000

static XAxiVdma videoDMAController;

// Now, let's get ready to start displaying some stuff on the screen.
// The variables framePointer and framePointer1 are just pointers to the base address
// of frame 0 and frame 1.
unsigned int * framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
//	unsigned int * framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR)
//		+ SCREEN_WIDTH
//			* SCREEN_HEIGHT;
void init() {
	// init alien positions and draw them
	// init tank position and lives and draw it
	// init bunker positions and todo: erosion and draw them
	// todo: init score (numbers)
	memset(framePointer0, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4); //clears screen

	tank = initTank(TANK_START_X, TANK_START_Y);
	aliens = initAliens(ALIENS_START_X, ALIENS_START_Y);

	// TODO: move this to aliens.c
	int i;
	for (i = 0; i < ALIENS_COL; i++) {
		aliens.frontRowAliens[i] = &aliens.aliens[ALIENS_ROW-1][i];
	}

	bunkers = initBunkers(BUNKER_START_X, BUNKER_START_Y);
	bullets = initBullets();

	initScore();
	drawGround();
	drawCharacters();
	drawTank(TANK_START_X, &tank);
	drawAliens(ALIENS_START_X, ALIENS_START_Y, &aliens);
	drawBunkers(BUNKER_START_X, BUNKER_START_Y);
	drawLives(&tank);
	//	render(); //needed only for changing the index of the frame buffer


//	tankPew(&tank, &bullets);
//	alienPew(&aliens, &bullets);
//	alienPew(&aliens, &bullets);
//	alienPew(&aliens, &bullets);
//	alienPew(&aliens, &bullets);
//	for (i = 0; i < 20; i++) {
//		updateBullets(&bullets);
//		volatile int j = 0;
//		while (j < 4000000) {
//			j++;
//		}
//	}

	// erode the bunkers
//	int j, row, col;
//	for (row = 0; row < 3; row++) {
//		for (col = 0; col < 4; col++) {
//			for (j = 0; j < 4; j++) { // 4 erosion levels
//				erodeBunker(0, row, col);
//				erodeBunker(1, row, col);
//				erodeBunker(2, row, col);
//				erodeBunker(3, row, col);
//				volatile int delay = 0;
//				while (delay++ < 1000000);
//			}
//		}
//	}

//	int j;
//	for(j = 0; j < 10; j++){
//		killAlien(&aliens.aliens[2][j]);
//	}
//	while (1) {
//		updateAliens(&aliens);
//		updateScore(10);
//		volatile int delay = 0;
//		while (delay++ < 2000000);
//	}
}

void drawGround() {
	int col;
	for (col = 0; col < SCREEN_WIDTH; col++) {
		framePointer0[GROUND_START_Y * SCREEN_WIDTH + col] = GREEN;
		framePointer0[(GROUND_START_Y + 1) * SCREEN_WIDTH + col] = GREEN;
	}
}


void render() {
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, 0, //the 0 is the frame index
			XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}

void edit_frameBuffer(Sprite *sp, Position *p) {
	int maxRow = (p->y + sp->height);
	int maxCol = (p->x + sp->width);
	int row, col;
	for (row = p->y; row < maxRow; row++) {
		for (col = p->x; col < maxCol; col++) {
			if (findPixelValue(p->x, p->y, col, row, sp)) {
				framePointer0[row * SCREEN_WIDTH + col] = sp->Color.color;
			} else {
				framePointer0[row * SCREEN_WIDTH + col] = 0x00000000;
			}
		}
	}

}

int findPixelValue(int x, int y, int col, int row, Sprite *sp) {
	int xval = (col - x) >> 1;
	int yval = (row - y) >> 1;
	int mask = 1 << ((sp->width >> 1) - 1);
	return (sp->sprite[yval] << xval) & mask;
}

void init_videoDMAController() {
	int Status; // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(
			XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if (XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController,
			VideoDMAConfig, XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
	}
	// Step 3: (optional) set the frame store number.
	if (XST_FAILURE == XAxiVdma_SetFrmStore(&videoDMAController, 2,
			XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.
	XAxiVdma_FrameCounter myFrameConfig;
	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount = 2;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if (Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is SCREEN_HEIGHT x SCREEN_WIDTH.
	XAxiVdma_DmaSetup myFrameBuffer;
	myFrameBuffer.VertSizeInput = SCREEN_HEIGHT; // SCREEN_HEIGHT vertical pixels.
	myFrameBuffer.HoriSizeInput = SCREEN_WIDTH * 4; // SCREEN_WIDTH horizontal (32-bit pixels).
	myFrameBuffer.Stride = SCREEN_WIDTH * 4; // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf = 1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if (XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ,
			&myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
	}
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	//	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4 * SCREEN_WIDTH * SCREEN_HEIGHT;

	if (XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController,
			XAXIVDMA_READ, myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");

	// Start the DMA for the read channel only.
	if (XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)) {
		xil_printf("DMA START FAILED\r\n");
	}
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, 0, //the 0 is the frame index
			XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, SCREEN_WIDTH*SCREEN_HEIGHT);
}
