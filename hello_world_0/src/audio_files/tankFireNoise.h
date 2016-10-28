/*
 * tankFireNoise.h
 *
 *  Created on: Oct 22, 2016
 *      Author: superman
 */

#ifndef TANKFIRENOISE_H_
#define TANKFIRENOISE_H_
#include <stdint.h>

//returns the sound rate of the .wav
int32_t getTankFireNoiseSoundRate();
//returns the number of bytes in the .wav
int32_t getTankFireNoiseSoundFrames();
//return the audio array
int32_t* getTankFireNoiseSound();
#endif /* TANKFIRENOISE_H_ */
