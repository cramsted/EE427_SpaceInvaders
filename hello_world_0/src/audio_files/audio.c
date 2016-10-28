/*
 *
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

#define NORMAL 0
#define END_OF_BUFFER 1

#define VOL_MAX     AC97_VOL_MAX
#define VOL_START   0x0404
#define VOL_MID     AC97_VOL_MID
#define VOL_MIN     AC97_VOL_MIN
#define VOL_MUTE    AC97_VOL_MUTE
#define VOL_CHANGE  0x0101

#define DUPLICATE_LOWER_2_BYTES(data) (data |= data << 16)

#define BUFFER_FILL_RATE 128
typedef struct {
    int32_t soundRate;  //rate that the sound plays at
    int32_t soundFrames; //how many sound bytes there are to play
    int32_t *audioFile; //pointer to the array of sound bytes
    int32_t currentPosition;    //furthest point of the sound array that has been loaded into the FIFO
    int32_t event;  //what event the sound is associated with
} Sound;

uint32_t audioEvents = 0; //audio events buffer
uint16_t volume;    //

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
    //creates the empty sound buffer
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

//Return a 1 if we reached end of the buffer
//Otherwise, return a zero
int32_t fillAudioBuffer(Sound *sound) {
    //maximum of bytes ever put in the FIFO
    int32_t iMax = BUFFER_FILL_RATE;
    int32_t end = 0;
    // Make sure we don't overflow
    if (sound->currentPosition + iMax > sound->soundFrames) {
        iMax = sound->soundFrames - sound->currentPosition;
        // Clear the audio event so it doesn't play again.
        clearAudioEvent(sound->event);
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
        return END_OF_BUFFER;
    }
    return NORMAL;
}

uint32_t eventPending(uint32_t event) {
    return audioEvents & event;
}

void playAudio() {
    //keeps track of what alien movement sound will be played next
    static int32_t alienMovementNumber = ALIEN_MOVEMENT_1;

    //priority determined by the position the event check for sound 
    if (eventPending(AUDIO_TANK_FIRE_NOISE)) { //priority 1
        fillAudioBuffer(&tankFireNoise);
    } else if (eventPending(AUDIO_TANK_EXPLOSION)) { //priority 2
        fillAudioBuffer(&tankExplosion);
    } else if (eventPending(AUDIO_EXPLOSION_ALIEN)) { //priority 3
        fillAudioBuffer(&explosionAlien);
    } else if (eventPending(AUDIO_UFO_NOISE)) { //priority 4
        fillAudioBuffer(&ufoNoise);
    } else if (eventPending(AUDIO_ALIEN_MOVEMENT)) { //priority 5
        // xil_printf("moveAlienSound\n\r");

        //fills the audio buffer with the appropriate sound, and then changes the 
        //sound to the next one in the sequence
        switch (alienMovementNumber) {
        case ALIEN_MOVEMENT_1:
            if (fillAudioBuffer(&alienMovement1) == END_OF_BUFFER) {
                alienMovementNumber = ALIEN_MOVEMENT_2;
            }
            break;
        case ALIEN_MOVEMENT_2:
            if (fillAudioBuffer(&alienMovement2) == END_OF_BUFFER) {
                alienMovementNumber = ALIEN_MOVEMENT_3;
            }
            break;
        case ALIEN_MOVEMENT_3:
            if (fillAudioBuffer(&alienMovement3) == END_OF_BUFFER) {
                alienMovementNumber = ALIEN_MOVEMENT_4;
            }
            break;
        case ALIEN_MOVEMENT_4:
            if (fillAudioBuffer(&alienMovement4) == END_OF_BUFFER) {
                alienMovementNumber = ALIEN_MOVEMENT_1;
            }
            break;
        default:
            break;
        }
    } else {
        //plays where there is no sound
        fillAudioBuffer(&noSound);
    }
}

void clearAudio() {
    clearAudioEvent(AUDIO_ALL_EVENTS);//clears all events
    playAudio(); //fills the buffer with no sound
}

//sets all of the volume controls to their default values
void writeVolume() {
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCBeepVol, volume);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, volume);
}

void increaseVolume() {
    // Only change volume if it doesn't go above the max allowed value
    if (volume - VOL_CHANGE >= VOL_MAX) {
        volume -= VOL_CHANGE;
        writeVolume(); //set volume
    }
}

void decreaseVolume() {
    // Only change volume if it doesn't go above the max allowed value
    if (volume + VOL_CHANGE <= VOL_MIN) {
        volume += VOL_CHANGE;
        writeVolume(); //set volume
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
    volume = VOL_START;
    writeVolume();
    //clear the audio FIFO buffer
    XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
    //set the control bit to allow IN_FIFO interrupts
    XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);

    //Initialize all the sounds
    initSounds();
}
