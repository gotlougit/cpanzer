#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include "objects.h"

#define WIDTH 1000 
#define HEIGHT 720
#define FRAMERATE 60
#define IMAGESCALE 0.9
#define TEXTURE_LOC "player.png"
#define WINNAME "Game Window"

const int SPEED = 10;
const uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
const uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

int close = 0;
textures *texlist = NULL;

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow(WINNAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, WINFLAGS);

	if (win == NULL) {
		printf("Error initializing window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, RENDFLAGS);

	if (rend == NULL) {
		printf("Error initalizing renderer: %s\n", SDL_GetError());
		return 1;
	}
	
	texlist = addTexture(texlist, rend, TEXTURE_LOC, "player",WIDTH/2,HEIGHT/2);
	texlist = addTexture(texlist, rend, "img.jpg", "enemy",WIDTH/2 - 100, HEIGHT/2 - 300);
	texlist = addTexture(texlist, rend, "snowmap.png","bg",0,0);
	
	while (!close) {
		
		/*Take input from the player and process it*/
		SDL_Event event;
		int dx = 0;
		int dy = 0;
		while (SDL_PollEvent(&event)) {
			
			switch (event.type) {
				
				case SDL_QUIT:
					close = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_Q:
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
	
		modRect(texlist, "player", dx, dy);
		checkBounds(texlist, WIDTH, HEIGHT);
	
		/*Clears the renderer and redraws the objects*/
		SDL_RenderClear(rend);
		renderTextures(texlist, rend);
		SDL_RenderPresent(rend);
		
		SDL_Delay(1000/FRAMERATE);

	}

	/*Clean up operations before quitting*/
	destroyTextures(texlist);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
