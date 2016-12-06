#pragma once
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
class Input;
class Draw;
class Font;
class Init1;
class Son;
class Joueur;
class map;
class menu;

class plateforme
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

private:

	GameObject plateforme1[50];
	SDL_Texture *plateformeTexture;
	int nombrePlateformes;
	int PLATEFORMES_MAX = 50;
	int PLATEFORM_SPEED = 2;
	int TILE_SIZE = 32;
	int UP = 3;
	int DOWN = 4;
	int RIGHT = 1;
	int LEFT = 2;

public:
	plateforme();
	~plateforme();
	void loadPlateforme(Draw *draw, Init1 *init);
	void cleanPlateforme(void);
	int getPlateformeNumber(void);
	void resetPlateformes(void);
	void initPlateforme(int x, int y, int type);
	void doPlateforme(Joueur *joueur);
	void drawPlateforme(map *map, Draw *draw, Init1 *init);
	void checkCollisionsWithPlateforms(GameObject * entity);
};

