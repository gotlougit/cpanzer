#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct textures {

	SDL_Texture *tex;
	SDL_Rect rect;
	char *texname;
	struct textures *next;
	int oldx;
	int oldy;
	int health;

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
		temp->oldx = x;
		temp->oldy = y;
		temp->health = 100;
		temp->next = list;
		list = temp;
		return list;
	}

}

void modRect(textures *list, char *texname, int dx, int dy) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->texname == texname) {
			temp->oldx = (temp->rect).x;
			temp->oldy = (temp->rect).y;
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
		temp->oldx = (temp->rect).x;
		temp->oldy = (temp->rect).y;
		(temp->rect).x = x;
		(temp->rect).y = y;
	}

}

int areCollidingY(textures *obj1, textures *obj2) {

	int y1 = (obj1->rect).y;
	int h1 = (obj1->rect).h;
	
	y1 += h1/2;

	int y2 = (obj2->rect).y;
	int h2 = (obj2->rect).h;

	y2 += h2/2;

	int result = 0;

	if (abs(y1-y2) <= (h1+h2)/2) {
		result = 1;
	}

	return result;

}

int areCollidingX(textures *obj1, textures *obj2) {

	int x1 = (obj1->rect).x;
	int w1 = (obj1->rect).w;

	x1 += w1/2;

	int x2 = (obj2->rect).x;
	int w2 = (obj2->rect).w;

	x2 += w2/2;

	int result = 0;

	if (abs(x1-x2) <= (w1+w2)/2) {
		result = 1;
	}

	return result;

}

void collisionAction(textures *obj) {

	(obj->rect).x = obj->oldx;
	(obj->rect).y = obj->oldy;

}

void checkCollision(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (strcmp(temp->texname,"bg") != 0) {

			for (textures *obj = list; obj != NULL; obj = obj->next) {
				if ( (obj != temp) && (strcmp(obj->texname,"bg") != 0) )  {
		
					int resultx = areCollidingX(obj, temp);		
					int resulty = areCollidingY(obj,temp);
					if (resultx && resulty) {
						collisionAction(obj);
						collisionAction(temp);
					}	
				}
			}
		}
	}
}
