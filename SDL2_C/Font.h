#pragma once
#include "input.h"
#include "Draw.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class Draw;
class menu;
class Init1;
class Son;
class Joueur;
class map;
class plateforme;

class Font
{
private:

	/* Déclaration de notre police de caractères */
	TTF_Font *font;

public:
	Font();
	~Font();
	void loadFont(char * name, int size);
	void closeFont();
	void drawString(char * text, int x, int y, int r, int g, int b, int a, Init1 *init);
};

