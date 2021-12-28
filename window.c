#include "objects.h"
#include "input.h"

#define MAIN 1
#include "constants.h"

typedef struct {
	int x;
	int y;
} randomStart;

randomStart getRandomCoords(void) {

	int rx = (rand() % (WIDTH/4 + 1));
	int ry = (rand() % ((HUDY - 2*COMPENSATION)/4 + 1)) + COMPENSATION;
	int decide = rand() % 4;

	switch (decide) {
		case 1:
			/*Upper right corner*/
			rx = WIDTH - rx;
			break;
		case 2:
			/*Lower left corner*/
			ry = HEIGHT - ry - COMPENSATION;
			break;
		case 3:
			/*Lower right corner*/
			rx = WIDTH - rx;
			ry = HEIGHT - ry - COMPENSATION;
			break;
		default:
			/*Upper left corner*/
			break;
	}

	randomStart out;
	out.x = rx;
	out.y = ry;
	
	return out;

}

bool close = false;
textures *texlist = NULL;
textures *implist = NULL;

int main(void) {

	printf("Loading game...\n");
	
	/*Initialize random seed*/
	srand(time(NULL));
	
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init()) {
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
	
	texlist = addTexture(texlist, rend, BASE_TEX, "base", WIDTH/2, HEIGHT/2);
	textures *base = texlist;
	base->rect.x -= base->rect.w/2;
	base->rect.y -= base->rect.h/2;

	texlist = addTexture(texlist, rend, HUD_TEX, "hud", WIDTH,HEIGHT);
	implist = addTexture(implist, rend, MAP_TEX,"bg",0,0);
	
	/*Variables for pausing the game*/
	bool pause = false;
	bool pauseRun = false;

	playInit();

	/*Game loop*/
	while (!close) {

		/*Close game if needed*/
		if (close) {
			printf("Closing game..\n");
			break;
		}
		
		if (base->health <= 0) {
			printf("Game Over!\n");
			printf("Your score was %d\n",player->points);
			break;
		}
	
		/*Take input from the player and process it*/
		inputResult input = getInput(pause);
		close = input.close;
		pause = input.pause;

		if (pause && !pauseRun) {
			pauseRun = true;
			printf("Paused the game\n");
			SDL_RenderClear(rend);
			/*The pause menu code here*/
			setTextBaseCoords(HEIGHT/2 - 150, 150);
			writeText(rend, RESUME_MESSAGE);
			writeText(rend, EXIT_MESSAGE);
			writeText(rend, EXIT2_MESSAGE);
			SDL_RenderPresent(rend);
		}

		else if (!pause) {
			
			if (pauseRun) {
				pauseRun = false;
				printf("Unpaused game\n");
			}

			int dx = input.value[0];
			int dy = input.value[1];
			int angle = input.value[2];
			int nozzleangle = input.value[3];
			bool projectile = input.projectile;

			/*Changes the coordinates of player*/
			modPlayer(player, dx, dy, angle);
			int a = modNozzle(texlist, nozzleangle, player);
		
			/*Launches projectiles if needed*/
			if (projectile && player->ammo) {
				texlist = addProjectile(texlist, rend, player, PROJ_TEX, a);
			}

			/*Updates all objects as per their functions*/
			updateEnemy(texlist,base->rect.x,base->rect.y);
			updateProjectile(texlist);
			
			/*Spawns enemies if needed*/
			for (int ec = countEnemy(texlist); ec < ENEMYCOUNT; ec++) {	
				
				randomStart random = getRandomCoords();	
				texlist = addTexture(texlist, rend, ENEMY_TEX, "enemy",random.x,random.y);
			}
			
			/*Checks for collisions*/
			checkCollision(texlist);
			/*Update points from projectile*/
			updatePoints(texlist, player);

			/*Checks for any deaths; if there are any, remove from linked list*/
			texlist = removeDead(texlist);
			/*Checks objects if they aren't out of bounds*/
			checkBounds(texlist, WIDTH, HEIGHT);
			
			/*Clears the renderer and redraws the objects*/
			SDL_RenderClear(rend);
			renderTextures(implist, rend);
			renderTextures(texlist, rend);

			/*Updates the HUD*/
			char statement[100];
			sprintf(statement, "Health: %0.0f | Points: %d | Ammo: %d",base->health,player->points,player->ammo);
			createText(rend, statement, HUDX, HUDY);

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
