#include <SDL2/SDL.h>
#include <stdbool.h>

/*Constants and includes for window.c*/
#ifdef MAIN

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_TEX "assets/player.png"
#define ENEMY_TEX "assets/bot2.png"
#define MAP_TEX "assets/snowmap.png"
#define HUD_TEX "assets/hud.png"
#define NOZZLE_TEX "assets/nozzle.png"
#define PROJ_TEX "assets/projectile.png"
#define BASE_TEX "assets/base.png"
#define ICON "assets/icon.png"
#define WINNAME "CPanzer"

uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

const int ENEMYCOUNT = 3;
const int WIDTH = 1000; 
const int HEIGHT = 720;
const int FRAMERATE = 60;
const int HUDX = 0;
const int HUDY = (HEIGHT-50);
const int COMPENSATION = 120;
const int MAXAMMO = 10;

#endif

/*Constants and includes specific to input.h*/
#ifdef INPUT

#define OUTPUTS 5
#define NOZZLESPEED 7
#define SPEED 17

#endif

/*Constants and includes specific to objects.h*/
#ifdef OBJECTS

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POINTINC 10
#define FONTLOC "assets/font.ttf"
#define FONTSIZE 42 
#define ENEMYSPEED 1
#define DAMAGE_RATE 0.01

#endif
