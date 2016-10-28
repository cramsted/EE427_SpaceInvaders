/*
 * tankExplosion.h
 *
 *  Created on: Oct 22, 2016
 *      Author: superman
 */

#ifndef TANKEXPLOSION_H_
#define TANKEXPLOSION_H_
#include <stdint.h>

//returns the sound rate of the .wav
int32_t getTankExplosionSoundRate();
//returns the number of bytes in the .wav
int32_t getTankExplosionSoundFrames();
//return the audio array
int32_t* getTankExplosionSound();
#endif /* TANKEXPLOSION_H_ */
