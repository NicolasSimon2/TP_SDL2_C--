#pragma once
#pragma once
#include <iostream>
#include <string>
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class Draw;
class Font;
class Init1;
class menu;
class Joueur;
class map;
class plateforme;

using namespace std;

enum class EnumSon
{
	BUMP,
	DEST,
	JUMP,
	STAR
};

class Son
{

private:
	int type;

public:
	Son();
	~Son();
	void loadSong(char filename[200]);
	void cleanUpMusic(void);
	void loadSound(void);
	void freeSound(void);
	void playSoundFx(int type);

};

