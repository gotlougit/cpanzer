#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define WIDTH 1280
#define HEIGHT 720
#define FRAMERATE 60
#define IMAGESCALE 6
#define TEXTURE_LOC "img.jpg"
#define WINNAME "Test Window"

const int SPEED = 300;
const uint32_t WINFLAGS = SDL_WINDOW_VULKAN;
const uint32_t RENDFLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

typedef struct {
	SDL_Texture *tex;
	SDL_Rect *rect;
} textureDict;

int close = 0;

SDL_Rect getRect(SDL_Renderer *rend, char *imageloc, int scale, int x, int y) {

	SDL_Surface *surface;
	surface = IMG_Load(imageloc);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
	if (tex == NULL) {
		printf("Error initalizing texture: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);
	
	SDL_Rect out;
	SDL_QueryTexture(tex, NULL, NULL, &out.w, &out.h);

	out.w = out.w / scale;
	out.h = out.h / scale;
	out.x = x - out.w;
	out.y = y - out.h;

	return out;
}

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

	SDL_Rect dest = getRect(rend, TEXTURE_LOC, IMAGESCALE, WIDTH/2, HEIGHT/2);
	SDL_Rect plain = getRect(rend, TEXTURE_LOC, 2*IMAGESCALE, WIDTH/4, HEIGHT/4);
	/*
	SDL_Surface *surface;
	surface = IMG_Load(TEXTURE_LOC);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
	if (tex == NULL) {
		printf("Error initalizing texture: %s\n", SDL_GetError());
		return 1;
	}

	SDL_FreeSurface(surface);

	SDL_Rect dest, plain;
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	SDL_QueryTexture(tex, NULL, NULL, &plain.w, &plain.h);

	plain.w = plain.w / (2*IMAGESCALE);
	plain.h = plain.h / (2*IMAGESCALE);
	plain.x = (WIDTH-plain.w)/4;
	plain.y = (HEIGHT-plain.h)/4;

	dest.w = dest.w/IMAGESCALE;
	dest.h = dest.h/IMAGESCALE;
	dest.x = (WIDTH-dest.w)/2;
	dest.y = (HEIGHT-dest.h)/2;

	*/

	while (!close) {
		
		SDL_Event event;

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
							dest.y -= SPEED / (FRAMERATE/2);
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							dest.x -= SPEED / (FRAMERATE/2);
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							dest.y += SPEED / (FRAMERATE/2);
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							dest.x += SPEED / (FRAMERATE/2);
							break;
						default:
							break;
					}
					break;


			}

		}
	
	if (dest.x + dest.w > WIDTH) {
		dest.x = WIDTH - dest.w;
	}

	if (dest.x < 0) {
		dest.x = 0;
	}

	if (dest.y + dest.h > HEIGHT) {
		dest.y = HEIGHT - dest.h;
	}
	
	if (dest.y < 0) {
		dest.y = 0;
	}

	SDL_RenderClear(rend);

	SDL_RenderCopy(rend, tex, NULL, &dest);
	SDL_RenderCopy(rend, tex, NULL, &plain);

	SDL_RenderPresent(rend);
	SDL_Delay(WIDTH/FRAMERATE);

	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
