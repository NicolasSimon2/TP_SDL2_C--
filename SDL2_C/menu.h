#pragma once
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "Plateforme.h"
class Input;
class Draw;
class Font;
class Init1;
class Son;
class Joueur;
class map;
class plateforme;

class menu
{

enum
{
	START,
	PAUSE
};

private :

	int onMenu, menuType, choice;
	SDL_Texture *titlescreen;
	int LEVEL_MAX = 2;


public:
	menu();
	~menu();
	int getOnMenu(void);
	int getMenuType(void);
	void setOnMenu(int valeur, int type);
	void initMenus(Init1 * init, Draw * draw);
	void cleanMenus(void);
	void updateStartMenu(Input *input, Joueur *joueur, map *map, plateforme *plateforme, Draw *draw, Init1 *init);
	void updatePauseMenu(Input * input);
	void drawStartMenu(Draw *draw, Init1 *init, Font *font, Joueur *joueur);
	void drawPauseMenu(Init1 *init, Font *font);
};

