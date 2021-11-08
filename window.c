#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include "objects.h"

#define WIDTH 1000 
#define HEIGHT 720
#define FRAMERATE 60
#define PLAYER_TEX "assets/player.png"
#define IMAGESCALE 0.9
#define ENEMY_TEX "assets/bot2.png"
#define MAP_TEX "assets/snowmap.png"
#define ICON "assets/icon.png"
#define WINNAME "CPanzer"

const int SPEED = 10;
const uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
const uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

int close = 0;
textures *texlist = NULL;

int main(void) {

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

	/*Sets a nice window icon*/
	SDL_Surface *surf;
	surf = IMG_Load(ICON);
	SDL_SetWindowIcon(win, surf);
	SDL_FreeSurface(surf);

	/*Create all of the objects required*/
	texlist = addTexture(texlist, rend, PLAYER_TEX, "player",WIDTH/2,HEIGHT/2);
	texlist = addTexture(texlist, rend, ENEMY_TEX, "enemy",WIDTH/2 - 100, HEIGHT/2 - 300);
	texlist = addTexture(texlist, rend, MAP_TEX,"bg",0,0);
	
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
		
		/*Changes the coordinates of player texture*/
		modRect(texlist, "player", dx, dy);

		/*Checks objects if they aren't out of bounds*/
		checkBounds(texlist, WIDTH, HEIGHT);
	
		/*Clears the renderer and redraws the objects*/
		SDL_RenderClear(rend);
		renderTextures(texlist, rend);
		SDL_RenderPresent(rend);
		
		/*Set framerate*/
		SDL_Delay(1000/FRAMERATE);

	}

	/*Clean up operations before quitting*/
	destroyTextures(texlist);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
