#define AUDIO 1

#include "constants.h"

void playEffect(char *effectName) {

	uint8_t *effectBuffer;
	uint32_t effectLen;
	SDL_AudioSpec effectSpec;

	SDL_LoadWAV(effectName, &effectSpec, &effectBuffer, &effectLen);
	SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &effectSpec, NULL, 0);

	if (!SDL_QueueAudio(deviceID, effectBuffer, effectLen)) {
		SDL_PauseAudioDevice(deviceID,0);
	} else {
		printf("Error playing sound effect %s: %s\n",effectName,SDL_GetError());
	}

	SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(effectBuffer);

}

void testSound(void) {

	playEffect(TESTAUDIO);

}
