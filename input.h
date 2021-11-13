#define INPUT 1

#include "constants.h"

typedef struct {
	int value[4];
} inputResult;

inputResult getInput(int SPEED, int pause) {

	int close = 0;
	int dx = 0;
	int dy = 0;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
				
			case SDL_QUIT:
				close = 1;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_P:
						pause = !pause;
						break;
					default:
						break;
				}
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						close = 1;
						break;
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_UP:
						dy -= SPEED;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						dx -= SPEED;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						dy += SPEED;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						dx += SPEED;
						break;
					default:
						break;
				}
	
				break;
		}

	}
	
	inputResult out;
	out.value[0] = dx;
	out.value[1] = dy;
	out.value[2] = close;
	out.value[3] = pause;
	return out;

}
