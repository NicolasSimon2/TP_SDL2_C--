#pragma once
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "menu.h"
#include "Plateforme.h"

class Input;
class Draw;
class Font;
class Init1;
class Son;
class Joueur;
class menu;
class plateforme;

class map
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

	typedef struct Map
	{

		SDL_Texture *background;
		SDL_Texture *tileSet, *tileSetB;

		//Numéro du tileset à utiliser
		int tilesetAffiche;

		/* Coordonnées de départ du héros, lorsqu'il commence le niveau */
		int beginx, beginy;

		/* Coordonnées de début, lorsqu'on doit dessiner la map */
		int startX, startY;

		/* Coordonnées max de fin de la map */
		int maxX, maxY;

		/* Tableau à double dimension représentant la map de tiles */
		int tile[150][400];

		//Deuxième couche de tiles
		int tile2[150][400];

		//Troisième couche de tiles
		int tile3[150][400];

		/* Timer et numéro du tileset à afficher pour animer la map */
		int mapTimer, tileSetNumber;

	} Map;

private:
	GameObject entity;
	Map map1;
	int MAX_MAP_X = 400;
	int MAX_MAP_Y = 150;
	int TILE_SIZE = 32;
	int LEVEL_MAX = 2;
	int BLANK_TILE = 89;
	int TILE_CHECKPOINT = 23;
	int TILE_POWER_UP_DEBUT = 67;
	int TILE_POWER_UP_FIN = 69;
	int TILE_TRAVERSABLE = 70;
	int TILE_RESSORT = 115;
	int JUMP_HEIGHT = 10;
	int TILE_PIKES = 117;
	int TILE_PLATEFORME_DEBUT = 120;
	int TILE_PLATEFORME_FIN = 121;
	int TIME_BETWEEN_2_FRAMES = 20;
	int SCREEN_HEIGHT = 480;
	int SCREEN_WIDTH = 800;


public:
	map();
	~map();

	int getStartX(void);

	void setStartX(int valeur);

	int getStartY(void);

	void setStartY(int valeur);

	int getMaxX(void);

	int getMaxY(void);

	int getBeginX(void);

	int getBeginY(void);

	SDL_Texture * getBackground(void);

	int getTileValue(int y, int x);

	SDL_Texture * getTileSetA(void);

	SDL_Texture * getTileSetB(void);

	void initMaps(Draw *draw, Init1 *init);

	void loadMap(char * name);

	void drawMap(int layer, plateforme *plateforme, Draw *draw, Init1 *init);

	void changeLevel(Draw *draw, Joueur *joueur, Init1 *init);

	void cleanMaps(void);

	void mapCollision(GameObject *entity, Joueur *joueur, Son *son, plateforme *plateforme, Draw *draw, Init1 *init, map *map);


};

