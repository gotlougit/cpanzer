#define INPUT 1

#include "constants.h"

typedef struct {
	int value[OUTPUTS];
	bool pause;
	bool projectile;
} inputResult;

inputResult getInput(bool pause) {

	int angle = 0;
	int close = 0;
	int dx = 0;
	int dy = 0;
	int nozzleangle = 0;
	bool projectile = false;

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
					case SDL_SCANCODE_SPACE:
						projectile = true;
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
						angle = 0;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						dx -= SPEED;
						angle = -90;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						dy += SPEED;
						angle = 180;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						dx += SPEED;
						angle = 90;
						break;
					case SDL_SCANCODE_E:
						nozzleangle += NOZZLESPEED;
						break;
					case SDL_SCANCODE_Q:
						nozzleangle -= NOZZLESPEED;
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
	out.pause = pause;
	out.projectile = projectile;
	out.value[3] = angle;
	out.value[4] = nozzleangle;
	return out;

}
