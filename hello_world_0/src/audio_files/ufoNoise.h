/*
 * ufoNoise.h
 *
 *  Created on: Oct 22, 2016
 *      Author: superman
 */

#ifndef UFONOISE_H_
#define UFONOISE_H_
#include <stdint.h>


//returns the sound rate of the .wav
int32_t getUfoNoiseSoundRate();
//returns the number of bytes in the .wav
int32_t getUfoNoiseSoundFrames();
//return the audio array
int32_t* getUfoNoiseSound();

#endif /* UFONOISE_H_ */
