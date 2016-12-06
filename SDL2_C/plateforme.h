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

		// Points de vie/santé + chrono d'invicibilité
		int life, invincibleTimer;

		// Coordonnées du sprite
		int x, y;

		// Largeur, hauteur du sprite 
		int h, w;

		// Checkpoint pour le héros (actif ou non)
		int checkpointActif;
		// + coordonnées de respawn (réapparition)
		int respawnX, respawnY;


		// Variables utiles pour l'animation :
		// Numéro de la frame (= image) en cours + timer
		int frameNumber, frameTimer, frameMax;
		// Nombre max de frames, état du sprite et direction
		//   dans laquelle il se déplace (gauche / droite)
		int etat, direction;


		// Variables utiles pour la gestion des collisions :
		//Est-il sur le sol, chrono une fois mort
		int onGround, timerMort;
		//Vecteurs de déplacement temporaires avant détection
		//des collisions avec la map
		float dirX, dirY;
		//Sauvegarde des coordonnées de départ
		int saveX, saveY;

		//Variable pour le double saut
		int jump;

		//Variables pour gérer les plateformes flottantes
		//Coordonnées de départ
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

