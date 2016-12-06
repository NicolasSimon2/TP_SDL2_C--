#pragma once
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class Draw;
class Font;
class Init1;
class Son;
class menu;
class map;
class plateforme;
class GameObject;

using namespace std;

class Joueur
{

	typedef struct GameObject
	{

		// Points de vie/sant� + chrono d'invicibilit�
		int life, invincibleTimer;

		// Coordonn�es du sprite
		int x, y;

		// Largeur, hauteur du sprite 
		int h, w;

		// Checkpoint pour le h�ros (actif ou non)
		int checkpointActif;
		// + coordonn�es de respawn (r�apparition)
		int respawnX, respawnY;


		// Variables utiles pour l'animation :
		// Num�ro de la frame (= image) en cours + timer
		int frameNumber, frameTimer, frameMax;
		// Nombre max de frames, �tat du sprite et direction
		//   dans laquelle il se d�place (gauche / droite)
		int etat, direction;


		// Variables utiles pour la gestion des collisions :
		//Est-il sur le sol, chrono une fois mort
		int onGround, timerMort;
		//Vecteurs de d�placement temporaires avant d�tection
		//des collisions avec la map
		float dirX, dirY;
		//Sauvegarde des coordonn�es de d�part
		int saveX, saveY;

		//Variable pour le double saut
		int jump;

		//Variables pour g�rer les plateformes flottantes
		//Coordonn�es de d�part
		int beginX, beginY;
		// Type de plateforme (horizontale ou verticale)
		// Le joueur est-il dessus ?
		int type, player_dessus;





	} GameObject;

public:
	Joueur();
	~Joueur();
	void getPlayer(GameObject gameobject);
	int getPlayerDirection(void);
	void resetCheckpoint(void);
	int getPlayerx(void);
	int getPlayery(void);
	void setPlayerx(int valeur);
	void setPlayery(int valeur);
	int getLevel(void);
	void SetValeurDuNiveau(int valeur);
	int getLife(void);
	int getNombreDeVies(void);
	void setNombreDeVies(int valeur);
	int getNombreDetoiles(void);
	void setNombreDetoiles(int valeur);
	void initPlayerSprites(Draw *draw, Init1 *init);
	void cleanPlayer(void);
	void killPlayer(Son *son);
	//void playerHurts(GameObject * monster, Son *son);
	void initializePlayer(int newLevel, plateforme *plateforme, map *map);
	void updatePlayer(Input *input, Son *son, menu *menu, map *map, Joueur *joueur, plateforme *plateforme, Draw *draw, Init1 *init);
	void drawPlayer(map *map, Init1 *init);
	void centerScrollingOnPlayer(map *map);
	void getItem(int itemNumber, Son *son);

private:
	int vies, etoiles, level;
	GameObject player;
	SDL_Texture *playerSpriteSheet;
	int JUMP_HEIGHT = 10;
	int RIGHT = 1;
	int IDLE = 0;
	int TIME_BETWEEN_2_FRAMES_PLAYER = 4;
	int PLAYER_WIDTH = 40;
	int PLAYER_HEIGTH = 50;
	int GRAVITY_SPEED = 0.6;
	int MAX_FALL_SPEED = 15;
	int PLAYER_SPEED = 4;
	int LEFT = 2;
	int WALK = 1;
	int JUMP1 = 2;
	int JUMP2 = 3;
	int LIMITE_X = 400;
	int LIMITE_Y = 220;
	int LIMITE_W = 100;
	int LIMITE_H = 80;
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 480;

	
};
