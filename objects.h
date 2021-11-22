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
	int oldx;
	int oldy;
	float health;
	int points;
	int angle;
	int oldangle;
	struct textures *next;

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
		temp->oldangle = 0;
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

void updateHUD(textures *list, SDL_Renderer *rend, int points, int health, int HUDX, int HUDY) {
	
	char statement[100];
	sprintf(statement, "Health: %d | Points: %d",health,points);
	textObj hud = createText(rend, statement,HUDX,HUDY);
	SDL_RenderCopy(rend,hud.tex,NULL,&(hud.rect));
	SDL_DestroyTexture(hud.tex);

}

int countEnemy(textures *list) {
	int count = 0;
	for (textures *iter = list; iter != NULL; iter = iter->next) {
		if (!strcmp(iter->texname,"enemy")) {
			count += 1;
		}
	}
	return count;
}

void modPlayer(textures *temp, int dx, int dy, int angle) {

	temp->oldx = (temp->rect).x;
	temp->oldy = (temp->rect).y;
	(temp->rect).x += dx;
	(temp->rect).y += dy;

	if (temp->angle != angle) {
		temp->angle = angle;
	}


}

void modNozzle(textures *list, int nozzleangle, textures *player) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (!strcmp(temp->texname,"nozzle")) {

			int playercx = (player->rect).x + (player->rect).w/2;
			int playercy = (player->rect).y + (player->rect).h/2;
			temp->oldx = (temp->rect).x;
			temp->oldy = (temp->rect).y;
			(temp->rect).x = playercx - temp->rect.w/2;
			(temp->rect).y = playercy - temp->rect.h/2;
			temp->oldangle += nozzleangle;
			if (temp->oldangle >= 360 || temp->oldangle <= -360) {
				temp->oldangle = 0;
			}
			temp->angle = player->angle - 90 + temp->oldangle;
			break;

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

void renderTextures(textures *list, SDL_Renderer *rend) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		SDL_RenderCopyEx(rend, temp->tex, NULL, &(temp->rect), temp->angle, NULL, SDL_FLIP_NONE);
	}

}

textures * updateEnemy(textures *list, int px, int py) {

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

bool areColliding(textures *obj1, textures *obj2) {

	int y1 = (obj1->rect).y;
	int h1 = (obj1->rect).h;
	int x1 = (obj1->rect).x;
	int w1 = (obj1->rect).w;

	x1 += w1/2;
	y1 += h1/2;

	int x2 = (obj2->rect).x;
	int w2 = (obj2->rect).w;
	int y2 = (obj2->rect).y;
	int h2 = (obj2->rect).h;

	x2 += w2/2;
	y2 += h2/2;

	bool result = false;

	if ((abs(y1-y2) <= (h1+h2)/2) && (abs(x1-x2) <= (w1+w2)/2)) {
		result = true;
	}

	return result;

}

void collisionAction(textures *obj, char *collideswith) {

	bool flag = false;
	if (!strcmp(obj->texname,"player")) {
		if (!strcmp(collideswith, "enemy")) {
			obj->points += POINTINC;
			
		}
		else if (!strcmp(collideswith, "base") && (obj->oldx == (obj->rect).x) && obj->oldy == (obj->rect).y) {
			flag = true;
			obj->health = 0;
		}
		else if (!strcmp(collideswith, "nozzle")) {
			flag = true;
		}
	}
	else if (!strcmp(obj->texname,"enemy")) {
		if (!strcmp(collideswith, "player")) {
			obj->health = 0;
		}
		else if (!strcmp(collideswith, "enemy")) {
			obj->health = 0;	
		}
	}
	else if (!strcmp(obj->texname, "nozzle")) {
		flag = true;
	}
	else if (!strcmp(obj->texname, "base")) {
		if (!strcmp(collideswith,"enemy")) {
			flag = true;
			obj->health -= DAMAGE_RATE;
		}
	}

	if (!flag) {
		(obj->rect).x = obj->oldx;
		(obj->rect).y = obj->oldy;
	}

}

textures * removeDead(textures *list) {

	textures *obj= list->next;
	textures *last = list;
	if (list->health) {
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
		return list;
	} else {
		textures *obj = list->next;
		SDL_DestroyTexture(list->tex);
		free(list);
		return obj;
	}

}

void checkCollision(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->health > 0) {

			for (textures *obj = list; obj != NULL; obj = obj->next) {
				if ( (obj != temp) && obj->health > 0 )  {
		
					bool result = areColliding(obj, temp);		

					if (result) {
						collisionAction(obj,temp->texname);
						if (obj->health) {
							collisionAction(temp,obj->texname);
						}
					}	
				}
			}
		}
	}
}
