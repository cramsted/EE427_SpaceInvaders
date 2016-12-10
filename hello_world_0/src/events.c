/*
 * events.c
 *
 *  Created on: Oct 8, 2016
 *      Author: superman
 */
#include "events.h"
#include <stdint.h>
#include <stdio.h>
#include "tank.h"           //for access to tank functions
#include "bullets.h"        //for access to bullet update functions
#include "text.h"           //for access to ufo explosion score thing
#include "bunkers.h"        //for access to bunker functions
#include "ufo.h"            //for access to ufo functions
#include "audio_files/audio.h"  //for access to the sound event setters and clearing fucntions
#include "pit.h"
#include <string.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xuartlite.h"
#include "render.h"
#include "dmacontroller.h"

// took the average of 45 samples
// the zero utilization count (excluding event handlers)
// this ignores the overhead of the interrupts
#define ZERO_UTILIZATION 5256055 // 4544399
#define MAX_STRING_SIZE 80

//values for turning on/off the screen shot frame buffer
#define SCREEN_SHOT_ON 1
#define SCREEN_SHOT_OFF 0

// Holds all pending events, where each event is a different bit
uint32_t events = 0;
uint32_t enabled = 1; //1 for enabled, 0 for diabled
uint32_t screen_shot_pause = 0; //0 for disabled, 1 for enabled
//used to calculate the utilization of the processor
uint32_t utilizationCounter = 0;

static XUartLite uart;

static int32_t isDigit(char c);
static uint32_t power(uint32_t base, int32_t exp);

//function declarations
void disableEvents();
void setScreenShotFreeze(uint32_t x);

//sets the passed in event
void setEvent(int event) {
	events |= event;
}
//clears the passed in event
void clearEvent(int event) {
	events &= ~event;
}
//checks for a left button event
//moves the tank left when activated
void leftButtonEvent() {
	if (events & LEFT_BTN_EVENT) {
		// Move tank left
		clearEvent(LEFT_BTN_EVENT);
		moveTankLeft(&tank);
	}
}
//checks for a right button event
//moves the tank right when activated
void rightButtonEvent() {
	if (events & RIGHT_BTN_EVENT) {
		// Move tank right
		clearEvent(RIGHT_BTN_EVENT);
		moveTankRight(&tank);
	}
}

//checks for a middle button event
//fires the tank bullet
void middleButtonEvent() {
	if (events & MIDDLE_BTN_EVENT) {
		// Fire tank bullet
		clearEvent(MIDDLE_BTN_EVENT);
		tankPew(&tank, &bullets);
	}
}
//updates the position of the bullets
void bulletRefreshEvent() {
	if (events & BULLETS_REFRESH_EVENT) {
		// Move bullets and do collision detecting
		clearEvent(BULLETS_REFRESH_EVENT);
		updateBullets(&bullets);
	}
}
//updates the position of the alien block
void aliensRefreshEvent() {
	if (events & ALIENS_REFRESH_EVENT) {
		// Move aliens
		clearEvent(ALIENS_REFRESH_EVENT);
		updateAliens(&aliens);
	}
}
//fires and alien bullet
void aliensFireEvent() {
	if (events & ALIENS_FIRE_EVENT) {
		// Aliens fire a bullet
		clearEvent(ALIENS_FIRE_EVENT);
		alienPew(&aliens, &bullets);
	}
}
//erases the explosion of an alien from the screen
void alienDeathEvent() {
	if (events & ALIEN_DEATH_EVENT) {
		// Erases a dead alien explosion from the screen
		clearEvent(ALIEN_DEATH_EVENT);
		eraseAlienExplosionSprite();
	}
}

//updates the position of the ufo
void ufoUpdateEvent() {
	if (events & UFO_UPDATE_EVENT) {
		clearEvent(UFO_UPDATE_EVENT);
		updateUfo();
	}
}

//erases the score sprite left behind by the downed ufo
void ufoExplosionEvent() {
	if (events & UFO_EXPLOSION_EVENT) {
		clearEvent(UFO_EXPLOSION_EVENT);
		ufoDisapear();
	}
}

//increments a counter that runs anytime there are no events
void heartbeatEvent() {
	//		float utilization = 0;
	if (events & HEARTBEAT_EVENT) {
		// "Idle" event - calculate utilization
		clearEvent(HEARTBEAT_EVENT);
		// TODO: remove the return and uncomment
		return;
		//			utilization = ((float) utilizationCounter / (float) ZERO_UTILIZATION);
		//			xil_printf("%d\n\r", (uint32_t) (100 * utilization));
		//			utilizationCounter = 0;
	}
}

//kills the tank and possibly ends game
void tankDeathEvent() {
	if (events & TANK_DEATH_EVENT) {
		disableAndClearEvents();
		//draws tank explosion sprite
		tankExplode();
		//checks if the game is over
		if (tank.lives == 0) {
			drawGameOver();
			clearAudio(0);
			while (1) {
			} //the game ends here
		}
	}
}

// call a function to draw the ufo at the top-left/right of the screen
void ufoAppearEvent() {
	if (events & UFO_APPEAR_EVENT) {
		clearEvent(UFO_APPEAR_EVENT);
		ufoAppear();
	}
}

//plays audion if an audio event has been triggered
void audioEvent() {
	if (events & AUDIO_EVENT) {
		clearEvent(AUDIO_EVENT);
		playAudio();
	}
}

void uartEvent() {
	if (events & UART_EVENT) {
		clearEvent(UART_EVENT);

		static char str[MAX_STRING_SIZE];     //character buffer
		static int i = 0;

		// Get input until they press enter
		uint8_t data;
		uint32_t bytesReceived = XUartLite_Recv(&uart, &data, 1); //try to retrieve one char
		if (bytesReceived == 0) { //check if char was received
			return;
		} else {
			str[i] = data;
			xil_printf("received: %c\n\r", str[i]);
		}

        //looks for end of the string
		if (str[i] == '\r') {
			int j;
			i = 0;

			uint32_t delayNumber = 0;
			uint32_t length = strlen(str); //get length of the char buffer
			for (j = length - 2; j >= 0; j--) {
				uint32_t ch = str[j];
				if (ch == '\r') {  //looks for the end of the input
					break;
				}
                //check input for numbers
				if (!isDigit(ch)) {
					xil_printf("You suck at numb3r5!\n\r");
					delayNumber = 0;
					break;
				}
                //convert the char to a digit of the correct power of ten
				ch -= '0';
				delayNumber += ch * power(10, length - j - 2);
			}
			xil_printf("\n\r Delay number = %d\n\r", delayNumber);
            //change the delay
			pitSetDelay(delayNumber);
            //reset the char buffer
			memset(str, 0, sizeof(str));
		} else {
			i++;
		}
	}
}
// switch 6 has been turned on
void switch_7_on_Event(){
	if (events & SW_7_ON_EVENT){
		xil_printf("sw 7 on\r\n");
		clearEvent(SW_7_ON_EVENT);
		dmaScreenShot();
	}
}

// switch 6 has been turned off
void switch_7_off_Event(){
	if (events & SW_7_OFF_EVENT){
		xil_printf("sw 7 off\r\n");
		clearEvent(SW_7_OFF_EVENT);
	}
}

// switch 6 has been turned on
void switch_6_on_Event(){
	if (events & SW_6_ON_EVENT){
		xil_printf("sw 6 on\r\n");
		clearEvent(SW_6_ON_EVENT);
		//freeze the game
		setScreenShotFreeze(SCREEN_SHOT_ON);
		//take the screen shot
		screenShot();
	}
}

// switch 6 has been turned off
void switch_6_off_Event(){
	if (events & SW_6_OFF_EVENT){
		xil_printf("sw 6 off\r\n");
		clearEvent(SW_6_OFF_EVENT);
		//unfreeze the game
		setScreenShotFreeze(SCREEN_SHOT_OFF);
	}
}

// switch 5 has been turned on
void switch_5_on_Event(){
	if (events & SW_5_ON_EVENT){
		xil_printf("sw 5 on\r\n");
		clearEvent(SW_5_ON_EVENT);
		//switch the frame buffer to the screen shot
		changeFrame(SCREEN_SHOT_ON);
		//update what buffer is being rendered
		render();
	}
}

// switch 5 has been turned off
void switch_5_off_Event(){
	if (events & SW_5_OFF_EVENT){
		xil_printf("sw 5 off\r\n");
		clearEvent(SW_5_OFF_EVENT);
		//switch the frame buffer to the normal game
		changeFrame(SCREEN_SHOT_OFF);
		//update what buffer is being rendered
		render();
	}
}

//returns 1 if events are enabled, 0 otherwise
int eventsEnabled() {
	return enabled;
}

void setScreenShotFreeze(uint32_t x){
	screen_shot_pause = x;
}

//returns if the system is paused due to a screen shot being taken
int screenShotEnabled(){
	return screen_shot_pause;
}

//disables events
void disableEvents(){
	enabled = 0;
}

//enables events
void enableEvents() {
	enabled = 1;
}

//disables and clears all events
void disableAndClearEvents() {
	enabled = 0;
	events = 0;
}

// Check all events round-robin style.
// if there are no events, it runs the utilization counter
void eventsLoop() {
	XUartLite_Initialize(&uart, XPAR_UARTLITE_1_DEVICE_ID);
	XUartLite_ResetFifos(&uart);
	pitSetDelay(PIT_INITIAL_DELAY);
	while (1) {
		if (events && enabled) {
			// An event is pending. Check all events round-robin style.
			leftButtonEvent();
			rightButtonEvent();
			middleButtonEvent();
			bulletRefreshEvent();
			aliensRefreshEvent();
			aliensFireEvent();
			alienDeathEvent();
			ufoUpdateEvent();
			tankDeathEvent();
			ufoAppearEvent();
			ufoExplosionEvent();
			heartbeatEvent();
			audioEvent();
			uartEvent();
			switch_7_on_Event();
			switch_7_off_Event();
			switch_6_on_Event();
			switch_6_off_Event();
			switch_5_on_Event();
			switch_5_off_Event();
//			xil_printf("left idle: %x\n\r", DMACONTROLLER_mReadSlaveReg3(XPAR_DMACONTROLLER_0_BASEADDR, DMACONTROLLER_SLV_REG3_OFFSET));
//			xil_printf("left read: %x\n\r", DMACONTROLLER_mReadSlaveReg4(XPAR_DMACONTROLLER_0_BASEADDR, DMACONTROLLER_SLV_REG4_OFFSET));
//			xil_printf("left wait for read: %x\n\r", DMACONTROLLER_mReadSlaveReg5(XPAR_DMACONTROLLER_0_BASEADDR, DMACONTROLLER_SLV_REG5_OFFSET));
//			xil_printf("left write: %x\n\r", DMACONTROLLER_mReadSlaveReg6(XPAR_DMACONTROLLER_0_BASEADDR, DMACONTROLLER_SLV_REG6_OFFSET));
//			xil_printf("left wait for write: %x\n\r", DMACONTROLLER_mReadSlaveReg7(XPAR_DMACONTROLLER_0_BASEADDR, DMACONTROLLER_SLV_REG7_OFFSET));
		} else {
			++utilizationCounter;
			// We used the following to get a baseline for utilization
			//			if (events & HEARTBEAT_EVENT) {
			//				events &= ~HEARTBEAT_EVENT;
			//				xil_printf("%d\n\r", utilizationCounter);
			//				utilizationCounter = 0;
			//			}
		}
	}
}

// We use this to validate the user's input
static int32_t isDigit(char c) {
	if (c < '0' || c > '9') {
		return 0;
	}
	return 1;
}

//replication of the math.pow function
//format is base^exp
static uint32_t power(uint32_t base, int32_t exp) {
	uint32_t i;
	uint32_t result = 1;
	for (i = 0; i < exp; i++) {
		result *= base;
	}
	return result;
}
