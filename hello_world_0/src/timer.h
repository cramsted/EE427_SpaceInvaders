/*
 * timer.h
 *
 *  Created on: Oct 4, 2016
 *      Author: superman
 */

#ifndef TIMER_H_
#define TIMER_H_

// Setup the FIT for use and initialize counters
void timerInit();

// The following functions are used by different parts of the program
// for timing purposes:

// Used when an alien explodes so the alien explosion will be erased 
void setAlienExplosionCounter();

// Used when the UFO dies so the points will be erased
void setUfoExplosionCounter();

// Used when the UFO is erased to reset the countdown until its next appearance
void resetUfoAppearanceCounter();

#endif /* TIMER_H_ */
