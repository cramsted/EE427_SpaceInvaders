/*
 * audio.c
 *
 *  Created on: Oct 26, 2016
 *      Author: superman
 */
#include "audio.h"
#include "../xac97_l.h"
#include "xparameters.h"
#include "alienMovement1.h"
#include "alienMovement2.h"
#include "alienMovement3.h"
#include "alienMovement4.h"
#include "explosionAlien.h"
#include "tankExplosion.h"
#include "tankFireNoise.h"
#include "ufoNoise.h"

#define TANK_EXPLOSION	0x1
#define TANK_FIRE_NOISE	0x2
#define UFO_NOISE		0x4
#define EXPLOSION_ALIEN	0x8
#define ALIEN_MOVEMENT	0x10

#define ALIEN_MOVEMENT_1 1
#define ALIEN_MOVEMENT_2 2
#define ALIEN_MOVEMENT_3 3
#define ALIEN_MOVEMENT_4 4

#define BUFFER_FILL_RATE 128
typedef struct {
	int32_t soundRate;
	int32_t soundFrames;
	int32_t *audioFile;
	int32_t currentPosition;
	int32_t event;
} Sound;

uint32_t audioEvents = 0;

//sound structs
Sound alienMovement1;
Sound alienMovement2;
Sound alienMovement3;
Sound alienMovement4;
Sound explosionAlien;
Sound tankExplosion;
Sound tankFireNoise;
Sound ufoNoise;

Sound initSound(int32_t soundRate, int32_t soundFrames, int32_t *audioFile,
		int32_t event) {
	Sound sound;
	sound.soundRate = soundRate;
	sound.soundFrames = soundFrames;
	sound.audioFile = audioFile;
	sound.currentPosition = 0;
	sound.event = event;
	return sound;
}

//initializes all of the sound structs
void initSounds() {
	alienMovement1 = initSound(getAienMovement_1_SoundRate(),
			getAienMovement_1_SoundFrames(), getAienMovement_1_Sound(),
			ALIEN_MOVEMENT);
	alienMovement2 = initSound(getAienMovement_2_SoundRate(),
			getAienMovement_2_SoundFrames(), getAienMovement_2_Sound(),
			ALIEN_MOVEMENT);
	alienMovement3 = initSound(getAienMovement_3_SoundRate(),
			getAienMovement_3_SoundFrames(), getAienMovement_3_Sound(),
			ALIEN_MOVEMENT);
	alienMovement4 = initSound(getAienMovement_4_SoundRate(),
			getAienMovement_4_SoundFrames(), getAienMovement_4_Sound(),
			ALIEN_MOVEMENT);
	explosionAlien = initSound(getExplosionAlienSoundRate(),
			getExplosionAlienSoundFrames(), getExplosionAlienSound(),
			EXPLOSION_ALIEN);
	tankExplosion = initSound(getTankExplosionSoundRate(),
			getTankExplosionSoundFrames(), getTankExplosionSound(),
			TANK_EXPLOSION);
	tankFireNoise = initSound(getTankFireNoiseSoundRate(),
			getTankFireNoiseSoundFrames(), getTankFireNoiseSound(),
			TANK_FIRE_NOISE);
	ufoNoise = initSound(getUfoNoiseSoundRate(), getUfoNoiseSoundFrames(),
			getUfoNoiseSound(), UFO_NOISE);
}

//set and audio event
void setAudioEvent(uint32_t event) {
	audioEvents |= event;
}

//clear and audio event
void clearAudioEvent(uint32_t event) {
	audioEvents &= ~event;
}

void fillAudioBuffer(Sound *sound) {
	//maximum of bytes ever put in the FIFO
	int iMax = BUFFER_FILL_RATE;
	// Make sure we don't overflow
	if (sound->currentPosition + iMax > sound->soundFrames) {
		iMax = sound->soundFrames - sound->currentPosition;
		// Reset index of the sound buffer back to zero
		sound->currentPosition = 0;
		// Clear the audio event so it doesn't play again.
		clearAudioEvent(sound->event);
	}

	int i;
	for (i = 0; i < iMax; i++) {
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, *(sound->audioFile+i));
	}
	sound->currentPosition += iMax;
}

int32_t eventStatus(int32_t event) {
	return audioEvents & event;
}

void playAudio() {
	static int32_t alienMovementNumber = 1;
	if (eventStatus(TANK_EXPLOSION)) {
		fillAudioBuffer(&tankExplosion);
	} else if (eventStatus(EXPLOSION_ALIEN)) {
		fillAudioBuffer(&explosionAlien);
	} else if (eventStatus(UFO_NOISE)) {
		fillAudioBuffer(&ufoNoise);
	} else if (eventStatus(TANK_FIRE_NOISE)) {
		fillAudioBuffer(&tankFireNoise);
	} else if (eventStatus(ALIEN_MOVEMENT)) {
		switch (alienMovementNumber) {
		case ALIEN_MOVEMENT_1:
			fillAudioBuffer(&alienMovement1);
			alienMovementNumber = ALIEN_MOVEMENT_2;
			break;
		case ALIEN_MOVEMENT_2:
			fillAudioBuffer(&alienMovement2);
			alienMovementNumber = ALIEN_MOVEMENT_3;
			break;
		case ALIEN_MOVEMENT_3:
			fillAudioBuffer(&alienMovement3);
			alienMovementNumber = ALIEN_MOVEMENT_4;
			break;
		case ALIEN_MOVEMENT_4:
			fillAudioBuffer(&alienMovement4);
			alienMovementNumber = ALIEN_MOVEMENT_1;
			break;
		default:
			break;
		}
	}
}

void initAudio() {
	//resets the sound controller
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	//waits until the codec can be written to
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	//enable different audio rates
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat,
			AC97_EXTENDED_AUDIO_CONTROL_VRA);
	//set to desired audio rate
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate,
			AC97_PCM_RATE_11025_HZ);
	//set the volumes
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, AC97_VOL_MID);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, AC97_VOL_MID);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCBeepVol, AC97_VOL_MID);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, AC97_VOL_MID);
	//clear the audio FIFO buffer
	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
	//set the control bit to allow IN_FIFO interupts
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
}
