#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

typedef struct textures {

	SDL_Texture *tex;
	SDL_Rect rect;
	char *texname;
	struct textures *next;

} textures;


textures * addTexture(textures *list, SDL_Renderer *rend, char *imageloc, char *texname, int x, int y) {

	SDL_Surface *surf;
	surf = IMG_Load(imageloc);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);

	if (tex == NULL) {
		printf("Error initalizing texture: %s\n", SDL_GetError());
		return NULL;
	}
	
	SDL_FreeSurface(surf);

	SDL_Rect rect;
	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;

	textures *temp = malloc(sizeof(textures));
	if (temp == NULL) {
		free(temp);
		return NULL;
	} else {
		temp->tex = tex;
		temp->rect = rect;
		temp->texname = texname;
		temp->next = list;
		list = temp;
		return list;
	}

}

void modRect(textures *list, char *texname, int dx, int dy) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->texname == texname) {
			(temp->rect).x += dx;
			(temp->rect).y += dy;
		}
	}

}

SDL_Rect getRect(textures *list, char *texname) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->texname == texname) {
			return temp->rect;
		}
	}

}

textures * destroyTextures(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		SDL_DestroyTexture(temp->tex);
	}

	textures *temp = NULL;
	while (list != NULL) {
		temp = list->next;
		free(list);
		list = temp;
	}
	return list;

}

SDL_Texture * getTexture(textures *list, char *texname) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->texname == texname) {
			return temp->tex;
		}
	}
	return NULL;

}

void renderTextures(textures *list, SDL_Renderer *rend) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		SDL_RenderCopy(rend, temp->tex, NULL, &(temp->rect));
	}

}

void checkBounds(textures *list, int WIDTH, int HEIGHT) {
	
	for (textures *temp = list; temp != NULL; temp = temp->next) {
		int x = (temp->rect).x;
		int y = (temp->rect).y;
		int w = (temp->rect).w;
		int h = (temp->rect).h;
		if (x + w > WIDTH) {
			x = WIDTH - w;
		}
		if (x < 0) {
			x = 0;
		}
		if (y + h > HEIGHT) {
			y = HEIGHT - h;
		}
		if (y < 0) {
			y = 0;
		}

		(temp->rect).x = x;
		(temp->rect).y = y;
	}

}
