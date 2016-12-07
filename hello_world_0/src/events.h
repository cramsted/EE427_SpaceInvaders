/*
 * events.h
 *
 *  Created on: Oct 4, 2016
 *      Author: superman
 */

#ifndef EVENTS_H_
#define EVENTS_H_

// Different kinds of events
#define LEFT_BTN_EVENT			0x01
#define RIGHT_BTN_EVENT			0x02
#define MIDDLE_BTN_EVENT		0x04
#define BULLETS_REFRESH_EVENT	0x08
#define ALIENS_REFRESH_EVENT	0x10
#define ALIENS_FIRE_EVENT		0x20
#define HEARTBEAT_EVENT			0x40
#define ALIEN_DEATH_EVENT		0x80
#define TANK_DEATH_EVENT		0x100
#define UFO_UPDATE_EVENT		0x200
#define UFO_APPEAR_EVENT		0x400
#define UFO_EXPLOSION_EVENT		0x800
#define AUDIO_EVENT				0x1000
#define UART_EVENT				0x2000
#define SW_6_ON_EVENT			0x4000
#define SW_6_OFF_EVENT			0x8000
#define SW_5_ON_EVENT			0x10000
#define SW_5_OFF_EVENT			0x20000

// Check all events round-robin style.
// if there are no events, it runs the utilization counter
void eventsLoop();
//sets the passed in event
void setEvent(int event);
//returns 1 if events are enabled, 0 otherwise
void enableEvents();
//disables and clears all events
void disableAndClearEvents();
//returns 1 if events are enabled, 0 otherwise
int eventsEnabled();
//returns if the system is paused due to a screen shot being taken
int screenShotEnabled();
#endif /* EVENTS_H_ */
