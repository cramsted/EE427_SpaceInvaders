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

// took the average of 45 samples
// the zero utilization count (excluding event handlers)
// this ignores the overhead of the interrupts
#define ZERO_UTILIZATION 5256055 // 4544399
// Holds all pending events, where each event is a different bit
uint32_t events = 0;
uint32_t enabled = 1; //1 for enabled, 0 for diabled

//used to calculate the utilization of the processor
uint32_t utilizationCounter = 0;

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
    // TODO: remove the return and uncomment
    return;

    //	float utilization = 0;
    //	if (events & HEARTBEAT_EVENT) {
    //		// "Idle" event - calculate utilization
    //		clearEvent(HEARTBEAT_EVENT);
    //		utilization = ((float) utilizationCounter / (float) ZERO_UTILIZATION);
    //		xil_printf("%d\n\r", (uint32_t) (100 * utilization));
    //		utilizationCounter = 0;
    //	}
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
//returns 1 if events are enabled, 0 otherwise
int eventsEnabled() {
    return enabled;
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
