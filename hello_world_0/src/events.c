/*
 * events.c
 *
 *  Created on: Oct 8, 2016
 *      Author: superman
 */
#include "events.h"
#include <stdint.h>
#include <stdio.h>
#include "tank.h"
#include "bullets.h"
#include "text.h"
#include "bunkers.h"
#include "ufo.h"
#include "audio_files/audio.h"

// took the average of 45 samples
// the zero utilization count (excluding event handlers)
// this ignores the overhead of the interrupts
#define ZERO_UTILIZATION 5256055 // 4544399
// Holds all pending events, where each event is a different bit
uint32_t events = 0;
uint32_t enabled = 1; //1 for enabled, 0 for diabled
//used to calculate the utilization of the processor
uint32_t utilizationCounter = 0;

void setEvent(int event) {
    events |= event;
}

void clearEvent(int event) {
    events &= ~event;
}

void leftButtonEvent() {
    if (events & LEFT_BTN_EVENT) {
        // Move tank left
        clearEvent(LEFT_BTN_EVENT);
        moveTankLeft(&tank);
    }
}

void rightButtonEvent() {
    if (events & RIGHT_BTN_EVENT) {
        // Move tank right
        clearEvent(RIGHT_BTN_EVENT);
        moveTankRight(&tank);
    }
}

void middleButtonEvent() {
    if (events & MIDDLE_BTN_EVENT) {
        // Fire tank bullet
        clearEvent(MIDDLE_BTN_EVENT);
        tankPew(&tank, &bullets);
    }
}

void bulletRefreshEvent() {
    if (events & BULLETS_REFRESH_EVENT) {
        // Move bullets and do collision detecting
        clearEvent(BULLETS_REFRESH_EVENT);
        updateBullets(&bullets);
    }
}

void aliensRefreshEvent() {
    if (events & ALIENS_REFRESH_EVENT) {
        // Move aliens
        clearEvent(ALIENS_REFRESH_EVENT);
        updateAliens(&aliens);
    }
}

void aliensFireEvent() {
    if (events & ALIENS_FIRE_EVENT) {
        // Aliens fire a bullet
        clearEvent(ALIENS_FIRE_EVENT);
        alienPew(&aliens, &bullets);
    }
}

void alienDeathEvent() {
    if (events & ALIEN_DEATH_EVENT) {
        // Erases a dead alien explosion from the screen
        clearEvent(ALIEN_DEATH_EVENT);
        eraseAlienExplosionSprite();
    }
}

void ufoUpdateEvent() {
    if (events & UFO_UPDATE_EVENT) {
        clearEvent(UFO_UPDATE_EVENT);
        updateUfo();
    }
}

void ufoExplosionEvent() {
    if (events & UFO_EXPLOSION_EVENT) {
        clearEvent(UFO_EXPLOSION_EVENT);
        ufoDisapear();
    }
}

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

void tankDeathEvent() {
    if (events & TANK_DEATH_EVENT) {
        disableAndClearEvents();
        tankExplode();
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

void audioEvent() {
    if (events & AUDIO_EVENT) {
        clearEvent(AUDIO_EVENT);
        playAudio();
    }
}

int eventsEnabled() {
    return enabled;
}

void enableEvents() {
    enabled = 1;
}

void disableAndClearEvents() {
    enabled = 0;
    events = 0;
}

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
