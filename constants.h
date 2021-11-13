#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_TEX "assets/player.png"
#define ENEMY_TEX "assets/bot2.png"
#define MAP_TEX "assets/snowmap.png"
#define HUD_TEX "assets/hud.png"
#define ICON "assets/icon.png"
#define WINNAME "CPanzer"

const int WIDTH = 1000; 
const int HEIGHT = 720;
const int FRAMERATE = 60;
const float IMAGESCALE = 0.9;
const int HUDHEIGHT = 150;
const int HUDX = 0;
const int HUDY = (HEIGHT-50);
const int SPEED = 16;
const int ENEMYSPEED = 1;
const int ENEMYCOUNT = 2;

const uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
const uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
