#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

typedef struct textures {

	SDL_Texture *tex;
	char *texname;
	struct textures *next;

} textures;

textures * addTexture(textures *list, SDL_Renderer *rend, char *imageloc, char *texname) {

	SDL_Surface *surf;
	surf = IMG_Load(imageloc);

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);

	if (tex == NULL) {
		printf("Error initalizing texture: %s\n", SDL_GetError());
		return NULL;
	}
	
	SDL_FreeSurface(surf);

	textures *temp = malloc(sizeof(textures));
	if (temp == NULL) {
		free(temp);
		return NULL;
	} else {
		temp->tex = tex;
		temp->texname = texname;
		temp->next = list;
		list = temp;
		return list;
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
