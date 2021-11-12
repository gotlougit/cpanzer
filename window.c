#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "objects.h"
#include "input.h"

#define WIDTH 1000 
#define HEIGHT 720
#define FRAMERATE 60
#define PLAYER_TEX "assets/player.png"
#define IMAGESCALE 0.9
#define ENEMY_TEX "assets/bot2.png"
#define MAP_TEX "assets/snowmap.png"
#define HUD_TEX "assets/hud.png"
#define ICON "assets/icon.png"
#define WINNAME "CPanzer"
#define HUDHEIGHT 150
#define HUDX 0
#define HUDY (HEIGHT-50)

const int SPEED = 16;
const int ENEMYSPEED = 1;
const int ENEMYCOUNT = 2;

const uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
const uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

int close = 0;
textures *texlist = NULL;

int main(void) {

	printf("Loading game...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) {
		printf("Error initializing SDL_TTF: %s\n", TTF_GetError());
		return 1;
	}

	/*Creates the main window*/

	SDL_Window *win = SDL_CreateWindow(WINNAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, WINFLAGS);

	if (win == NULL) {
		printf("Error initializing window: %s\n", SDL_GetError());
		return 1;
	}

	/*Creates the SDL_Renderer object*/

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

	int randomx[ENEMYCOUNT];
	int randomy[ENEMYCOUNT];

	for (int i = 0; i < ENEMYCOUNT; i++) {

		int rx = (rand() % (WIDTH - HUDHEIGHT + 1)) + HUDHEIGHT;
		int ry = (rand() % (HEIGHT - HUDHEIGHT + 1)) + HUDHEIGHT;

		int flag = 0;

		for (int j = 0; j < ENEMYCOUNT; j++) {
			if (randomx[j] == rx) {
				flag = 1;
			}
			if (randomy[j] == ry) {
				flag = 1;
			}
		}
		
		if (!flag) {
			texlist = addTexture(texlist, rend, ENEMY_TEX, "enemy",rx,ry);
		} else {
			i-=1;
		}
	}

	texlist = addTexture(texlist, rend, HUD_TEX, "hud", WIDTH,HEIGHT);
	texlist = addTexture(texlist, rend, MAP_TEX,"bg",0,0);

	/*Create the HUD*/
	textObj hud = createText(rend, "Points: 0",HUDX,HUDY);

	/*Variables for pausing the game*/
	int pause = 0;
	int pauseRun = 0;

	/*Game loop*/
	while (!close) {
		
		/*Take input from the player and process it*/
		inputResult input = getInput(SPEED, pause);
		close = input.value[2];
		pause = input.value[3];

		if (close) {
			printf("Closing game..\n");
			break;
		}

		if (pause && !pauseRun) {
			pauseRun = 1;
			printf("Paused the game\n");
		}

		else if (!pause) {
			
			if (pauseRun) {
				pauseRun = 0;
				printf("Unpaused game\n");
			}

			int dx = input.value[0];
			int dy = input.value[1];

			/*Changes the coordinates of player texture*/
			modRect(texlist, "player", dx, dy);

			/*Updates all objects as per their functions*/
			int px = getRect(texlist, "player").x;
			int py = getRect(texlist, "player").y;
			texlist = updateEnemy(texlist, ENEMYSPEED,px,py);

			/*Checks for collisions*/
			checkCollision(texlist);

			/*Checks for any deaths; if there are any, remove from linked list*/
			removeDead(texlist);

			/*Checks objects if they aren't out of bounds*/
			checkBounds(texlist, WIDTH, HEIGHT);

			/*Clears the renderer and redraws the objects*/
			SDL_RenderClear(rend);
			renderTextures(texlist, rend);
			int points = getPoints(texlist);
			int health = getHealth(texlist);
			char statement[50];
			sprintf(statement, "Health: %d | Points: %d",health,points);
			/*TODO Get rid of magic numbers in HUD Rendering*/
			textObj hud = createText(rend, statement,HUDX,HUDY);
			SDL_RenderCopy(rend,hud.tex,NULL,&(hud.rect));
			SDL_RenderPresent(rend);
			
			/*Set framerate*/
			SDL_Delay(1000/FRAMERATE);

		}

	}

	/*Clean up operations before quitting*/
	destroyTextures(texlist);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
