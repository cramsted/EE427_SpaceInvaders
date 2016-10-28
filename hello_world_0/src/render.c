/*
 * render.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "render.h"
#include <stdio.h>
#include "sprites.h"    //for access to sprite structs
#include "xaxivdma.h"
#include "xparameters.h"
#include "xio.h"
#include "aliens.h"     //for access to alien initializers
#include "bullets.h"    //for access to bullet initializer
#include "bunkers.h"    //for access to bunkers initializers
#include "ufo.h"        //for access to ufo initializer
#include "tank.h"       //for access to tank initializer
#include "text.h"       //for access to text related initializers

//function prototypes
int32_t findPixelValue(int32_t x, int32_t y, int32_t col, int32_t row, Sprite *sp);
void drawGround();

// Starting location in DDR where we will store the images that we display.
#define FRAME_BUFFER_0_ADDR 0xC3000000

static XAxiVdma videoDMAController;

// Now, let's get ready to start displaying some stuff on the screen.
// The variables framePointer and framePointer1 are just pointers to the base address
// of frame 0 and frame 1.
unsigned int32_t * framePointer0 = (unsigned int32_t *) FRAME_BUFFER_0_ADDR;
//	unsigned int32_t * framePointer1 = ((unsigned int32_t *) FRAME_BUFFER_0_ADDR)
//		+ SCREEN_WIDTH
//			* SCREEN_HEIGHT;

// init alien positions and draw them
// init tank position and lives and draw it
// init bunker positions and erosion and draw them
// init score (numbers)
void videoInit() {

	memset(framePointer0, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4); //clears screen

	tank = initTank(TANK_START_X, TANK_START_Y); //creates tank struct
	initAliens(ALIENS_START_X, ALIENS_START_Y); //creates aliens block struct
	bunkers = initBunkers(BUNKER_START_X, BUNKER_START_Y); //creates bunkers block
	bullets = initBullets(); //creates bullets struct
	initUfo();

	initScore(); //initializes and draws score
	drawGround();
	drawCharacters(); //draws words like 'score' and 'lives'
	drawTank(TANK_START_X, &tank); //draws the tank
	drawAliens(0, 0); //draws aliens block
	drawBunkers(BUNKER_START_X, BUNKER_START_Y); //draws bunkers
	drawLives(); //draws the tank shaped lives
	//	render(); //needed only for changing the index of the frame buffer
}

//draws a horizonatal green line accross the bottom of the screen
void drawGround() {
	int32_t col;
	for (col = 0; col < SCREEN_WIDTH; col++) {
		//the line is two pixel wide
		framePointer0[GROUND_START_Y * SCREEN_WIDTH + col] = GREEN;
		framePointer0[(GROUND_START_Y + 1) * SCREEN_WIDTH + col] = GREEN;
	}
}

//switches beteen two different frame buffers
//not currently in use
void render() {
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, 0, //the 0 is the frame index
			XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}

//changes the values in the frame buffer array given a sprite and a postion it needs to be drawn at
void editFrameBuffer(Sprite *sp, Position *p) {
	int32_t maxRow = (p->y + sp->height);
	int32_t maxCol = (p->x + sp->width);
	int32_t row, col;
	for (row = p->y; row < maxRow; row++) {
		for (col = p->x; col < maxCol; col++) {
			unsigned int32_t *temp = &framePointer0[row * SCREEN_WIDTH + col];
			//if sprite value is 1 then it draws the sprite's color at that pixel
			if (findPixelValue(p->x, p->y, col, row, sp)) {
				*temp = sp->Color.color;
			} else { //if sprite value is a 0 it draws black
				*temp = BLACK;
			}
		}
	}
}

//based on the current x,y coordinate that the edit_frameBuffer function is on and based on the
//x,y position of the sprite, the sprite is resized, and the pixel value at that row and col
//is returned, 0 for black, 1 for with color
int32_t findPixelValue(int32_t x, int32_t y, int32_t col, int32_t row, Sprite *sp) {
	//causes each pixel to be drawn twice
	int32_t xval = (col - x) >> 1;
	int32_t yval = (row - y) >> 1;
	int32_t mask = 1 << ((sp->width >> 1) - 1); //makes a mask that will cover the desired pixel
	return (sp->sprite[yval] << xval) & mask; //returns pixel value
}

//This is part of Dr. Hutchings's code
//sets up the video DMA controller
void initVideoDMAController() {
	int32_t Status; // Keep track of success/failure of system function calls.
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
