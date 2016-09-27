/*
 * render.c
 *
 *  Created on: Sep 23, 2016
 *      Author: superman
 */
#include "render.h"
#include <stdio.h>
#include "sprites.h"
#include "xaxivdma.h"
#include "xparameters.h"
#include "xio.h"
#include "globals.h"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
//function prototypes
void edit_frameBuffer(Sprite *sp, Position *p);
int findPixelValue(int x, int y, int col, int row, Sprite *sp);
void drawAliens(int x, int y);
void drawBunkers(int x, int y);
void drawTank(int x);
void drawStaticImages();
void drawCharacters();
void drawLives();

#define FRAME_BUFFER_0_ADDR 0xC0000000  // Starting location in DDR where we will store the images that we display.
static XAxiVdma videoDMAController;
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
// Now, let's get ready to start displaying some stuff on the screen.
// The variables framePointer and framePointer1 are just pointers to the base address
// of frame 0 and frame 1.
unsigned int * framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
//	unsigned int * framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + SCREEN_WIDTH
//			* SCREEN_HEIGHT;
void init() {
	// TODO: initialize various structs, etc.
	// init alien positions and draw them
	// init tank position and lives and draw it
	// init bunker positions and erosion and draw them
	// init score
	tank = initTank(TANK_START_X, TANK_START_Y);
	aliens = initAliens(ALIENS_START_X, ALIENS_START_Y);
	bunkers = initBunkers(BUNKER_START_X, BUNKER_START_Y);
	drawStaticImages();
	drawTank(TANK_START_X);
	drawAliens(ALIENS_START_X, ALIENS_START_Y);
	drawBunkers(BUNKER_START_X, BUNKER_START_Y);
	drawLives();
	//	render(); //needed only for changing the index of the frame buffer
	int i;
	//	for( i = TANK_START_X; i < SCREEN_WIDTH - tank.sp.width; i++){
	//		drawTank(i);
	//		volatile unsigned int j = 0;
	//		while(j < 400000){
	//			j++;
	//		}
	//	}
	int end = SCREEN_WIDTH - ((aliens.aliens[0][0].sp.width + 8) * 10);
	for (i = ALIENS_START_X; i < end; i++) {
		drawAliens(i, ALIENS_START_Y);
		volatile unsigned int j = 0;
		while (j < 100000) {
			j++;
		}
	}
}

void drawStaticImages() {
	memset(framePointer0, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4); //clears screen
	int col;
	for (col = 0; col < SCREEN_WIDTH; col++) {
		framePointer0[GROUND_START_Y * SCREEN_WIDTH + col] = GREEN;
		framePointer0[(GROUND_START_Y + 1) * SCREEN_WIDTH + col] = GREEN;
	}
	drawCharacters();
}

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
void drawLives() {
	Tank life = initTank(LIFE_START_X, LIFE_START_Y);
	int col;
	for (col = 0; col < MAX_LIVES; col++) {
		if (col < tank.lives) {
			tank.sp.Color.color = GREEN;
		} else {
			tank.sp.Color.color = BLACK;
		}
		life.p.x = LIFE_START_X + (col * XLIFE_PADDING) + col * life.sp.width;
		edit_frameBuffer(&life.sp, &life.p);
	}
}
void drawTank(int x) {
	tank.sp.Color.color = BLACK;
	edit_frameBuffer(&tank.sp, &tank.p);
	tank.sp.Color.color = GREEN;
	tank.p.x = x;
	edit_frameBuffer(&tank.sp, &tank.p);
}

void drawAliens(int x, int y) {
	int row, col;
	for (row = 0; row < ALIENS_ROW; row++) {
		for (col = 0; col < ALIENS_COL; col++) {
			Alien *temp = &aliens.aliens[row][col];
			temp->sp.Color.color = BLACK;
			edit_frameBuffer(&temp->sp, &temp->p);
			temp->sp.Color.color = WHITE;
			temp->p.x = x + (temp->sp.width + 8) * col;
			edit_frameBuffer(&temp->sp, &temp->p);
		}
	}
}

void drawBunkers(int x, int y) {
	int row;
	for (row = 0; row < MAX_BUNKERS; row++) {
		Bunker *temp = &bunkers.bunkers[row];
		edit_frameBuffer(&temp->sp, &temp->p);
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