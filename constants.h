#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define MAXAMMO 10
#define PI 3.14159265358979323846

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

#define RESUME_MESSAGE "Press P to resume the game"
#define EXIT_MESSAGE "Press Esc to close the game"
#define EXIT2_MESSAGE "(no progress will be saved!)"

#define ENEMYCOUNT 3
#define WIDTH 1000
#define HEIGHT 720
#define FRAMERATE 60
#define HUDX 0
#define HUDY (HEIGHT-50)
#define COMPENSATION 120

uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

#endif

/*Constants and includes specific to input.h*/
#ifdef INPUT

#define OUTPUTS 4
#define NOZZLESPEED 7
#define SPEED 10

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
#define PROJ_SPEED 10
#define DAMAGE_RATE 0.01
#define AMMOCHANCE 2

#endif

/*Constants and includes specific to audio.h*/
#ifdef AUDIO

#include <stdlib.h>
#include <SDL2/SDL_mixer.h>

#define INIT_SOUND "assets/CarStart.wav"
#define EXPLODE_SOUND "assets/explode.wav"
#define FIRING_SOUND "assets/Gunfire.wav"
#define AUDIO_ENABLE 1

#endif
