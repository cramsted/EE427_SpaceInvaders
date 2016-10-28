/*
 * explosionAlien.h
 *
 *  Created on: Oct 22, 2016
 *      Author: superman
 */

#ifndef EXPLOSIONALIEN_H_
#define EXPLOSIONALIEN_H_
#include <stdint.h>

//returns the sound rate of the .wav
int32_t getExplosionAlienSoundRate();
//returns the number of bytes in the .wav
int32_t getExplosionAlienSoundFrames();
//return the audio array
int32_t* getExplosionAlienSound();
#endif /* EXPLOSIONALIEN_H_ */
