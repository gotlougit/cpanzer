#define OBJECTS 1

#include "audio.h"
#include "constants.h"

const SDL_Color WHITE = {255,255,255};
const SDL_Color RED = {255,0,0};
const SDL_Color GREEN = {0,255,0};
const SDL_Color BLUE = {0,0,255};

SDL_Color color = WHITE;

int font_x = 0;
int font_y = 0;

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
	int ammo;
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
		temp->ammo = 0;
		temp->next = list;
		list = temp;
		return list;
	}

}

textures * addProjectile(textures *list, SDL_Renderer *rend, textures *player, char * PROJ_TEX, int angle) {

	playFiring();
	float rx = ((player->rect.w) * cos(PI*angle/180));
	float ry = ((player->rect.h) * sin(PI*angle/180));
	int x = player->rect.x + player->rect.w/2 + rx;
	int y = player->rect.y + player->rect.h/2 + ry;
	list = addTexture(list, rend, PROJ_TEX, "projectile",x,y);
	list->rect.x -= list->rect.w/2;
	list->rect.y -= list->rect.h/2;
	list->angle = angle;
	player->ammo -= 1;
	
	return list;

}

void setTextBaseCoords(int x, int y) {

	font_x = x;
	font_y = y;

}

void createText(SDL_Renderer *rend, char *text, int x, int y) {

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

	TTF_CloseFont(font);

	SDL_RenderCopy(rend, tex, NULL, &rect);
	SDL_DestroyTexture(tex);

}

void writeText(SDL_Renderer *rend, char *text) {
	
	createText(rend, text, font_x, font_y);
	font_y += FONTSIZE;
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

	if (temp->angle != angle && !(!dx && !dy)) {
		temp->angle = angle;
	}


}

int modNozzle(textures *list, int nozzleangle, textures *player) {

	int angle = 0;

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
			angle = temp->angle;
			break;

		}
	}

	return angle;

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
		if (temp->health) {
			if (SDL_RenderCopyEx(rend, temp->tex, NULL, &(temp->rect), temp->angle, NULL, SDL_FLIP_NONE)) {
				printf("Error rendering texture %s: %s\n", temp->texname, SDL_GetError());
			}
		}
	}

}

void updateEnemy(textures *list, int px, int py) {

	for (textures *iter = list; iter != NULL; iter = iter->next) {
		if (!strcmp(iter->texname,"enemy")) {
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
	
}

void updateProjectile(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (!strcmp(temp->texname,"projectile")) {
			temp->oldx = temp->rect.x;
			temp->oldy = temp->rect.y;
			temp->rect.x += PROJ_SPEED * cos(PI * temp->angle / 180);
			temp->rect.y += PROJ_SPEED * sin(PI * temp->angle / 180);
		}
	}

}

void updatePoints(textures *list, textures *player) {
	
	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->points && !strcmp(temp->texname,"projectile")) {
			player->points += temp->points;
		}
	}

}

void checkBounds(textures *list, int WIDTH, int HEIGHT) {
	
	for (textures *temp = list; temp != NULL; temp = temp->next) {

		bool flag = false;

		int x = (temp->rect).x;
		int y = (temp->rect).y;
		int w = (temp->rect).w;
		int h = (temp->rect).h;

		if (x + w > WIDTH) {
			x = WIDTH - w;
			flag = true;
		}
		if (x < 0) {
			x = 0;
			flag = true;
		}
		if (y + h > HEIGHT) {
			y = HEIGHT - h;
			flag = true;
		}
		if (y < 0) {
			y = 0;
			flag = true;
		}

		if ((!strcmp(temp->texname,"projectile") || !strcmp(temp->texname,"enemy")) && flag) {
			temp->health = 0;
		}

		temp->oldx = (temp->rect).x;
		temp->oldy = (temp->rect).y;
		(temp->rect).x = x;
		(temp->rect).y = y;
	}

}

textures * findPredecessor(textures *list, textures *obj) {

	for (;list != NULL; list = list->next) {
		if (list->next == obj) {
			return list;
		}
	}
	return NULL;
	
}

textures * removeDead(textures *list) {

	textures *obj= list;
	if (list->health) {
		while (obj != NULL) {
			if (!obj->health) {
				textures *last = findPredecessor(list,obj);
				last->next = obj->next;
				SDL_DestroyTexture(obj->tex);
				free(obj);
				textures *obj = last->next;
			}
			obj = obj->next;
		}
		return list;
	} else {
		textures *obj = list->next;
		SDL_DestroyTexture(list->tex);
		free(list);
		return obj;
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

void collisionAction(textures *obj, textures *otherobj) {

	/*One liner evaluates if we need to make the objects stay where they are or not
	 * Nozzle is an exception since it needs to stick with player at all times */
	bool flag = (!(!strcmp(obj->texname,"nozzle") || !strcmp(otherobj->texname, "nozzle")) || !strcmp(obj->texname, otherobj->texname));

	/*Special cases for each object interaction*/

	if (!strcmp(obj->texname,"player")) {
		if (!strcmp(otherobj->texname, "enemy")) {
			playExplode();
			if (obj->ammo < MAXAMMO && !(rand() % AMMOCHANCE)) {
				obj->ammo += 1;	
			}
			if (!otherobj->points) {
				obj->points += POINTINC;
			}
			otherobj->points = POINTINC;
			otherobj->health = 0;
		}
	}

	else if (!strcmp(obj->texname, "projectile")) {
		obj->health = 0;
		if (!strcmp(otherobj->texname, "enemy")) {
			playExplode();
			otherobj->health = 0;
			obj->points = POINTINC;
			otherobj->points = POINTINC;
		}
	}

	else if (!strcmp(obj->texname, "base")) {
		if (!strcmp(otherobj->texname,"enemy")) {
			obj->health -= DAMAGE_RATE;
		}
	}

	else if (!strcmp(obj->texname, "enemy") && !strcmp(otherobj->texname, "enemy")) {
		obj->health = 100;
		otherobj->health = 0;
	}

	if (flag) {
		(obj->rect).x = obj->oldx;
		(obj->rect).y = obj->oldy;
	}

}

void checkCollision(textures *list) {

	for (textures *temp = list; temp != NULL; temp = temp->next) {
		if (temp->health > 0) {

			for (textures *obj = temp->next; obj != NULL; obj = obj->next) {
				if ( obj->health > 0 )  {
		
					if (areColliding(obj,temp)) {
						collisionAction(obj,temp);
						if (obj->health) {
							collisionAction(temp, obj);
						}
					}
				}
			}
		}
	}
}
