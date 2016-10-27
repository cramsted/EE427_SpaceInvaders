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
#include "../events.h"
#include <stdio.h>

#define ALIEN_MOVEMENT_1 1
#define ALIEN_MOVEMENT_2 2
#define ALIEN_MOVEMENT_3 3
#define ALIEN_MOVEMENT_4 4

#define DUPLICATE_LOWER_2_BYTES(data) (data |= data << 16)

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
Sound noSound;

int32_t noAudio[BUFFER_FILL_RATE];

Sound initSound(int32_t soundRate, int32_t soundFrames, int32_t *audioFile,
        uint32_t event) {
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
            AUDIO_ALIEN_MOVEMENT);
    alienMovement2 = initSound(getAienMovement_2_SoundRate(),
            getAienMovement_2_SoundFrames(), getAienMovement_2_Sound(),
            AUDIO_ALIEN_MOVEMENT);
    alienMovement3 = initSound(getAienMovement_3_SoundRate(),
            getAienMovement_3_SoundFrames(), getAienMovement_3_Sound(),
            AUDIO_ALIEN_MOVEMENT);
    alienMovement4 = initSound(getAienMovement_4_SoundRate(),
            getAienMovement_4_SoundFrames(), getAienMovement_4_Sound(),
            AUDIO_ALIEN_MOVEMENT);
    explosionAlien = initSound(getExplosionAlienSoundRate(),
            getExplosionAlienSoundFrames(), getExplosionAlienSound(),
            AUDIO_EXPLOSION_ALIEN);
    tankExplosion = initSound(getTankExplosionSoundRate(),
            getTankExplosionSoundFrames(), getTankExplosionSound(),
            AUDIO_TANK_EXPLOSION);
    tankFireNoise = initSound(getTankFireNoiseSoundRate(),
            getTankFireNoiseSoundFrames(), getTankFireNoiseSound(),
            AUDIO_TANK_FIRE_NOISE);
    ufoNoise = initSound(getUfoNoiseSoundRate(), getUfoNoiseSoundFrames(),
            getUfoNoiseSound(), AUDIO_UFO_NOISE);
    int32_t i;
    for (i = 0; i < BUFFER_FILL_RATE; i++) {
        noAudio[i] = BUFFER_FILL_RATE;
    }
    noSound = initSound(0, BUFFER_FILL_RATE, noAudio, 0);
}

//set an audio event
void setAudioEvent(uint32_t event) {
    audioEvents |= event;
}

//clear an audio event
void clearAudioEvent(uint32_t event) {
    audioEvents &= ~event;
}

void fillAudioBuffer(Sound *sound) {
    //maximum of bytes ever put in the FIFO
    int32_t iMax = BUFFER_FILL_RATE;
    int32_t end = 0;
    // Make sure we don't overflow
    if (sound->currentPosition + iMax > sound->soundFrames) {
        iMax = sound->soundFrames - sound->currentPosition;
        // Clear the audio event so it doesn't play again.
        clearAudioEvent(sound->event);
        xil_printf("clear audio %d\n\r", sound->event);
        end = 1;
    }

    int32_t i;
    for (i = 0; i < iMax; i++) {
        // Duplicate the lower 2 bytes in the upper 2 bytes for stereo
        uint32_t data = *(sound->audioFile + sound->currentPosition + i);
        DUPLICATE_LOWER_2_BYTES(data);
        XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, data);
    }
    sound->currentPosition += iMax;

    if (end) {
        // Reset index of the sound buffer back to zero
        sound->currentPosition = 0;
    }
}

uint32_t eventPending(uint32_t event) {
    return audioEvents & event;
}

void playAudio() {
    xil_printf("%d | ", audioEvents);
    static int32_t alienMovementNumber = 1;
        if (eventPending(AUDIO_ALIEN_MOVEMENT)) {
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
    } else if (eventPending(AUDIO_TANK_FIRE_NOISE)) {
        fillAudioBuffer(&tankFireNoise);
    } else if (eventPending(AUDIO_TANK_EXPLOSION)) {
        fillAudioBuffer(&tankExplosion);
    } else if (eventPending(AUDIO_EXPLOSION_ALIEN)) {
        fillAudioBuffer(&explosionAlien);
    } else if (eventPending(AUDIO_UFO_NOISE)) {
        fillAudioBuffer(&ufoNoise);
    } else {
        fillAudioBuffer(&noSound);
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
    //set the control bit to allow IN_FIFO interrupts
    XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);

    //Initialize all the sounds
    initSounds();
}
