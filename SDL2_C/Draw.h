#pragma once
#include "input.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class menu;
class Font;
class Init1;
class Son;
class Joueur;
class map;
class plateforme;

class Draw
{

private:
	//HUD
	SDL_Texture *HUD_vie, *HUD_etoiles;
	int TILE_SIZE = 32;
	int TILE_POWER_UP_COEUR = 68;
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 480;

public:
	Draw();
	~Draw();
	void drawGame(int pauseMenu, map *map, Joueur *joueur, plateforme *plateforme, menu *menu, Init1 *init, Draw *draw, Font *font);
	SDL_Texture * loadImage(char * name, Init1 *init);
	void drawImage(SDL_Texture * image, int x, int y, Init1 *init);
	void drawImagePlus(SDL_Texture * image, int x, int y, double rotation, SDL_Point center, SDL_RendererFlip flip, Init1 *init);
	void drawTile(SDL_Texture * image, int destx, int desty, int srcx, int srcy, Init1 *init);
	void delay(unsigned int frameLimit);
	void drawHud(Joueur *joueur, map *map, Font *font, Init1 *init);
	void initHUD(Init1 *init);
	void cleanHUD(void);
};

