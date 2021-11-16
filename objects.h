#define OBJECTS 1

#include "constants.h"

const SDL_Color WHITE = {255,255,255};
const SDL_Color RED = {255,0,0};
const SDL_Color GREEN = {0,255,0};
const SDL_Color BLUE = {0,0,255};

SDL_Color color = WHITE;

typedef struct {
	SDL_Texture *tex;
	SDL_Rect rect;
} textObj;

typedef struct textures {

	SDL_Texture *tex;
	SDL_Rect rect;
	char *texname;
	struct textures *next;
	int oldx;
	int oldy;
	int health;
	int points;
	int angle;

} textures;

SDL_Texture * makeTexture(SDL_Renderer *rend, SDL_Surface *surf) {

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);

	if (tex == NULL) {
		printf("Error initalizing texture: %s\n", SDL_GetError());
		return NULL;
	}
	
	return tex;

}

textures * addTexture(textures *list, SDL_Renderer *rend, char *imageloc, char *texname, int x, int y) {

	SDL_Surface *surf;
	surf = IMG_Load(imageloc);

	SDL_Texture *tex = makeTexture(rend,surf);
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
		temp->points = 0;
		temp->angle = 0;
		temp->next = list;
		list = temp;
		return list;
	}

}

textObj createText(SDL_Renderer *rend, char *text, int x, int y) {

	TTF_Font *font = TTF_OpenFont(FONTLOC,FONTSIZE);
	if (font == NULL) {
		printf("Error opening font: %s\n",TTF_GetError());
	}

	SDL_Surface *surf = TTF_RenderText_Solid(font, text, color);
	int textwidth = surf->w;
	int textheight = surf->h;
	if (surf == NULL) {
		printf("Error creating text surface: %s\n",TTF_GetError());
	}
	SDL_Texture *tex = makeTexture(rend,surf);
	SDL_FreeSurface(surf);

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = textwidth; 
	rect.h = textheight;

	textObj out;
	out.tex = tex;
	out.rect = rect;

	TTF_CloseFont(font);

	return out;

}

void updateHUD(textures *list, SDL_Renderer *rend, int HUDX, int HUDY) {

	int points = getPoints(list);
	int health = getHealth(list);
	char statement[50];
	sprintf(statement, "Health: %d | Points: %d",health,points);
	textObj hud = createText(rend, statement,HUDX,HUDY);
	SDL_RenderCopy(rend,hud.tex,NULL,&(hud.rect));
	SDL_DestroyTexture(hud.tex);

}

void modRect(textures *list, char *texname, int dx, int dy, int angle) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->texname == texname) {
			temp->oldx = (temp->rect).x;
			temp->oldy = (temp->rect).y;
			(temp->rect).x += dx;
			(temp->rect).y += dy;

			if (temp->angle != angle) {
				temp->angle = angle;
			}

		}
	}

}

int getHealth(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (strcmp(temp->texname,"player") == 0) {
			return temp->health;
		}
	}

}

int getAngle(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (strcmp(temp->texname,"player") == 0) {
			return temp->angle;
		}
	}

}

int getPoints(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (strcmp(temp->texname,"player") == 0) {
			return temp->points;
		}
	}

}

SDL_Rect getRect(textures *list, char *texname) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (strcmp(temp->texname,texname) == 0) {
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
		SDL_RenderCopyEx(rend, temp->tex, NULL, &(temp->rect), temp->angle, NULL, SDL_FLIP_NONE);
	}

}

textures * updateEnemy(textures *list,int ENEMYSPEED, int px, int py) {

	for (textures *iter = list; iter != NULL; iter = iter->next) {
		if (strcmp(iter->texname,"enemy") == 0) {
			int ex = (iter->rect).x;
			int ey = (iter->rect).y;

			int dx = 0;
			int dy = 0;

			if (ex > px) {
				dx = -ENEMYSPEED;	
			} else if (ex < px) {
				dx = ENEMYSPEED;	
			}	
			else if (ey > py) {
				dy = -ENEMYSPEED;
			} else if (ey < py) {
				dy = ENEMYSPEED;
			}
			
			(iter->oldx) = ex;
			(iter->oldy) = ey;
			(iter->rect).x += dx;
			(iter->rect).y += dy;
			
		}
	}
	
	return list;
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

void collisionAction(textures *obj, char *collideswith) {

	bool isenemy = strcmp(obj->texname,"enemy") == 0 && strcmp(collideswith,"player") == 0;
	bool isplayer = strcmp(obj->texname,"player") == 0 && strcmp(collideswith,"enemy") == 0;

	if (isplayer) {
		obj->points+=POINTINC;
	}
	else if (isenemy) {
		obj->health = 0;
	} else {
		(obj->rect).x = obj->oldx;
		(obj->rect).y = obj->oldy;
	}

}

void removeDead(textures *list) {

	textures *obj= list->next;
	textures *last = list;
	while (obj != NULL) {
		if (obj->health == 0) {
			last->next = obj->next;
			textures *temp = obj->next;	
			SDL_DestroyTexture(obj->tex);
			free(obj);
			textures *obj = temp;
		
		}
		obj = obj->next;
		last = last->next;
	}

}

void checkCollision(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if ((strcmp(temp->texname,"bg") != 0) && temp->health > 0) {

			for (textures *obj = list; obj != NULL; obj = obj->next) {
				if ( (obj != temp) && (strcmp(obj->texname,"bg") != 0) && obj->health > 0 )  {
		
					int resultx = areCollidingX(obj, temp);		
					int resulty = areCollidingY(obj,temp);

					if (resultx && resulty) {
						collisionAction(obj,temp->texname);
						collisionAction(temp,obj->texname);
					}	
				}
			}
		}
	}
}
