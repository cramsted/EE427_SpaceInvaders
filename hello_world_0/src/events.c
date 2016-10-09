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

// took the average of 45 samples
// the zero utilization count (excluding event handlers)
// this ignores the overhead of the interrupts
#define ZERO_UTILIZATION 4544399
#define EVENTS_ENABLED 1

// Holds all pending events, where each event is a different bit
uint32_t events = 0;

void setEvent(int event) {
	events |= event;
}

void clearEvent(int event) {
	events &= ~event;
}

void eventsLoop() {
	uint32_t utilizationCounter = 0;
	float utilization = 0;
	// Events loop:
	while (1) {
		if (events && EVENTS_ENABLED) {
			// An event is pending...
			if (events & LEFT_BTN_EVENT) {
				// Move tank left
				events &= ~LEFT_BTN_EVENT;
				moveTankLeft(&tank);
			}
			if (events & RIGHT_BTN_EVENT) {
				// Move tank right
				events &= ~RIGHT_BTN_EVENT;
				moveTankRight(&tank);
			}
			if (events & MIDDLE_BTN_EVENT) {
				// Fire tank bullet
				events &= ~MIDDLE_BTN_EVENT;
				tankPew(&tank, &bullets);
			}
			if (events & BULLETS_REFRESH_EVENT) {
				// Move bullets and do collision detecting
				events &= ~BULLETS_REFRESH_EVENT;
				updateBullets(&bullets);
			}
			if (events & ALIENS_REFRESH_EVENT) {
				// Move aliens
				events &= ~ALIENS_REFRESH_EVENT;
				updateAliens(&aliens);
			}
			if (events & ALIENS_FIRE_EVENT) {
				// Aliens fire a bullet
				events &= ~ALIENS_FIRE_EVENT;
				alienPew(&aliens, &bullets);
			}
			if (events & ALIEN_DEATH_EVENT) {
				// Erases a dead alien explosion from the screen
				clearEvent(ALIEN_DEATH_EVENT);
				eraseAlienExplosionSprite();
			}
			if (events & HEARTBEAT_EVENT) {
				// "Idle" event - calculate utilization
				events &= ~HEARTBEAT_EVENT;
				utilization = ((float) utilizationCounter
						/ (float) ZERO_UTILIZATION);
				xil_printf("%d\n\r", (uint32_t) (100 * utilization));
				utilizationCounter = 0;
			}
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
