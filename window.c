#include "objects.h"
#include "input.h"

#define MAIN 1
#include "constants.h"

int close = 0;
textures *texlist = NULL;
textures *implist = NULL;

int main(void) {

	printf("Loading game...\n");

	srand(time(NULL));
	
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

	texlist = addTexture(texlist, rend, NOZZLE_TEX, "nozzle", WIDTH/2,HEIGHT/2);
	texlist = addTexture(texlist, rend, PLAYER_TEX, "player",WIDTH/2,HEIGHT/2);
	textures *player = getPlayer(texlist);
	texlist = addTexture(texlist, rend, HUD_TEX, "hud", WIDTH,HEIGHT);
	implist = addTexture(implist, rend, MAP_TEX,"bg",0,0);
	
	/*Variables for pausing the game*/
	bool pause = false;
	bool pauseRun = false;
	
	/*Game loop*/
	while (!close) {
		
		/*Take input from the player and process it*/
		int angle = player->angle;
		inputResult input = getInput(SPEED, pause, angle);
		close = input.value[2];
		pause = input.pause;

		if (close) {
			printf("Closing game..\n");
			break;
		}

		if (pause && !pauseRun) {
			pauseRun = true;
			printf("Paused the game\n");
			SDL_RenderClear(rend);
			/*Add pause menu code here*/
			SDL_RenderPresent(rend);
		}

		else if (!pause) {
			
			if (pauseRun) {
				pauseRun = false;
				printf("Unpaused game\n");
			}

			int dx = input.value[0];
			int dy = input.value[1];
			angle = input.value[3];

			/*Changes the coordinates of player texture*/
			modRect(texlist, "player", dx, dy, angle);
			modNozzle(texlist,angle);
			
			/*Updates all objects as per their functions*/
			int px = player->rect.x;
			int py = player->rect.y;
			
			for (int ec = countEnemy(texlist); ec < ENEMYCOUNT; ec++) {	
	
				int rx = (rand() % (WIDTH + 1)) + COMPENSATION;
				int ry = (rand() % (HUDY - 2*COMPENSATION + 1)) + COMPENSATION;
					
				texlist = addTexture(texlist, rend, ENEMY_TEX, "enemy",rx,ry);

			}
	
			texlist = updateEnemy(texlist, ENEMYSPEED,px,py);
			
			/*Checks for collisions*/
			checkCollision(texlist);

			/*Checks for any deaths; if there are any, remove from linked list*/
			texlist = removeDead(texlist);

			/*Checks objects if they aren't out of bounds*/
			checkBounds(texlist, WIDTH, HEIGHT);

			/*Clears the renderer and redraws the objects*/
			SDL_RenderClear(rend);
			renderTextures(implist, rend);
			renderTextures(texlist, rend);
			
			/*Updates the HUD*/
			int points = player->points;
			int health = player->health;
			updateHUD(texlist, rend, points, health, HUDX, HUDY);

			SDL_RenderPresent(rend);
		
			/*Set framerate*/
			SDL_Delay(1000/FRAMERATE);

		}

	}

	/*Clean up operations before quitting*/
	destroyTextures(texlist);
	destroyTextures(implist);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
