#pragma once
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class menu;
class Draw;
class Font;
class Init1;
class Son;
class Joueur;
class map;
class plateforme;

class Input
{

typedef struct Input1
{

	int left, right, up, down, jump, attack, enter, erase, pause;

} Input1;

private:

	//Manette
	SDL_Joystick *joystick;
	int DPADinUse = 0;
	int BOUTON_HAUT = 0;
	int BOUTON_BAS = 1;
	int BOUTON_GAUCHE = 2;
	int BOUTON_DROITE = 3;
	int BOUTON_SAUT = 10;
	int BOUTON_ATTAQUE = 12;
	int BOUTON_PAUSE = 4;
	int BOUTON_QUIT = 5;
	int DEAD_ZONE = 8000;

public:
	Input();
	~Input();
	void openJoystick(void);
	void closeJoystick(void);
	void gestionInputs(Input * input);
	void getInput(Input * input);
	int getDown();
	int getUp();
	int getRight();
	int getLeft();
	int getDelete();
	int getC();
	int getEnter();
	void setDownZ();
	void setUpZ();
	void setRightZ();
	void setLeftZ();
	void setDeleteZ();
	void setEnterZ();
	void setCZ();
	void getJoystick(Input * input);
	Input1 Input1;
};

