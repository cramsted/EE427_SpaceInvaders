/*
 * audio.h
 *
 *  Created on: Oct 26, 2016
 *      Author: superman
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdint.h>

// audio events
#define AUDIO_TANK_EXPLOSION    0x1
#define AUDIO_TANK_FIRE_NOISE   0x2
#define AUDIO_UFO_NOISE         0x4
#define AUDIO_EXPLOSION_ALIEN   0x8
#define AUDIO_ALIEN_MOVEMENT    0x10

void playAudio();
void setAudioEvent(uint32_t event);
void clearAudioEvent(uint32_t event);
void initAudio();

#endif /* AUDIO_H_ */
