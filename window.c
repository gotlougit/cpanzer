#include "objects.h"
#include "input.h"

#define MAIN 1
#include "constants.h"

int close = 0;
textures *texlist = NULL;
textures *implist = NULL;

int main(void) {

	printf("Loading game...\n");
	
	/*Initialize random seed*/
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

	texlist = addTexture(texlist, rend, NOZZLE_TEX, "nozzle", WIDTH/2-250,HEIGHT/2-250);
	texlist = addTexture(texlist, rend, PLAYER_TEX, "player",WIDTH/2-250,HEIGHT/2-250);
	textures *player = texlist;
	player->ammo = MAXAMMO;
	texlist = addTexture(texlist, rend, BASE_TEX, "base", WIDTH/2, HEIGHT-HUDY);
	textures *base = texlist;
	texlist = addTexture(texlist, rend, HUD_TEX, "hud", WIDTH,HEIGHT);
	implist = addTexture(implist, rend, MAP_TEX,"bg",0,0);
	
	/*Variables for pausing the game*/
	bool pause = false;
	bool pauseRun = false;
	
	/*Game loop*/
	while (!close) {
		
		/*Take input from the player and process it*/
		inputResult input = getInput(pause);
		close = input.value[2];
		pause = input.pause;

		if (close) {
			printf("Closing game..\n");
			break;
		}

		if (base->health <= 0) {
			printf("Game Over!\n");
			printf("Your score was %d\n",player->points);
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
			int angle = input.value[3];
			int nozzleangle = input.value[4];
			bool projectile = input.projectile;

			/*Changes the coordinates of player texture*/
			modPlayer(player, dx, dy, angle);
			modNozzle(texlist, nozzleangle, player);
		
			/*Launches projectiles if needed*/
			if (projectile && player->ammo) {
				int x = player->rect.x + 10;
				int y = player->rect.y + 10;
				texlist = addTexture(texlist, rend, PROJ_TEX, "projectile", x,y);
				texlist->angle = angle;
				player->ammo -= 1;
			}

			/*Updates all objects as per their functions*/
			int px = base->rect.x;
			int py = base->rect.y;
			
			for (int ec = countEnemy(texlist); ec < ENEMYCOUNT; ec++) {	
	
				int rx = (rand() % (WIDTH + 1)) + COMPENSATION;
				int ry = (rand() % (HUDY - 2*COMPENSATION + 1)) + COMPENSATION;
					
				texlist = addTexture(texlist, rend, ENEMY_TEX, "enemy",rx,ry);

			}
	
			texlist = updateEnemy(texlist,px,py);
			texlist = updateProjectile(texlist);
			
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
			int health = base->health;
			int ammo = player->ammo;
			updateHUD(texlist, rend, points, health, ammo, HUDX, HUDY);

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
