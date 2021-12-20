#define AUDIO 1

#include "constants.h"

void playEffect(char *effectName) {

	int audioRate = 44100;
	uint16_t audioFormat = AUDIO_S16SYS;
	int audioChannels = 2;
	int audioBuffers = 4096;

	
	if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers)) {
		printf("Error initializing audio: %s\n",Mix_GetError());
	}

	Mix_Chunk *sound = NULL;
	sound = Mix_LoadWAV(effectName);
	if (sound == NULL) {
		printf("Error loading sound effect %s: %s\n",effectName, Mix_GetError());
	}

	int channel = Mix_PlayChannel(-1, sound, 0);
	if (channel == -1) {
		printf("Error playing sound effect %s: %s\n",effectName, Mix_GetError());
	}
	
	while (!Mix_Playing(channel)) {
		Mix_FreeChunk(sound);
		Mix_CloseAudio();
	}

}

/* Convenient wrappers for playEffect */

void playInit(void) {
	playEffect(INIT_SOUND);
}

void playExplode(void) {
	playEffect(EXPLODE_SOUND);
}

void playFiring(void) {
	playEffect(FIRING_SOUND);
}
