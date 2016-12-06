#pragma once
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class Draw;
class Font;
class menu;
class Son;
class Joueur;
class map;
class plateforme;

class Init1
{

private:
	SDL_Window *screen;
	SDL_Renderer *renderer;
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 480;

public:
	Init1();
	~Init1();
	SDL_Renderer * getrenderer(void);
	void init(char * title, Font *font, Input *input);
	void loadGame(map * map, menu * menu, Joueur * joueur, Draw * draw, plateforme * plateforme, Son * son, Init1 * init);
	void cleanup(map *map, menu *menu, Joueur *joueur, Draw *draw, plateforme *plateforme, Son *son, Input *input);
};

