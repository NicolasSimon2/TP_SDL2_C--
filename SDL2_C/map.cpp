#include "stdio.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "menu.h"
#include "Plateforme.h"


map::map()
{
}


map::~map()
{
}




int map::getStartX(void)
{
	;
	return map1.startX;
}

void map::setStartX(int valeur)
{
	;
	map1.startX = valeur;
}

int map::getStartY(void)
{
	;
	return map1.startY;
}

void map::setStartY(int valeur)
{
	;
	map1.startY = valeur;
}

int map::getMaxX(void)
{
	;
	return map1.maxX;
}

int map::getMaxY(void)
{
	;
	return map1.maxY;
}

int map::getBeginX(void)
{
	;
	return map1.beginx;
}

int map::getBeginY(void)
{
	;
	return map1.beginy;
}


SDL_Texture *map::getBackground(void)
{
	return map1.background;
}

int map::getTileValue(int y, int x)
{
	return map1.tile[y][x];
}


SDL_Texture *map::getTileSetA(void)
{
	;
	return map1.tileSet;
}

SDL_Texture *map::getTileSetB(void)
{
	;
	return map1.tileSetB;
}



void map::initMaps(Draw *draw, Init1 *init)
{
	// Charge l'image du fond (background)  
	map1.background = draw->loadImage("graphics/background.png", init);

	//On initialise le timer
	map1.mapTimer = TIME_BETWEEN_2_FRAMES * 3;
	map1.tileSetNumber = 0;

}


void map::loadMap(char *name)
{
	;
	int x, y;
	FILE *fp;
	errno_t err;

	//Ouvre le fichier en lecture (renvoie une erreur s'il n'existe pas)
	if ((err = fopen_s(&fp, name, "rb")) != 0)
	{
		fprintf(stderr, "Le fichier map n'a pas pu etre ouvert.\n");
		exit(1);
	}


	fscanf_s(fp, "%d", &map1.beginx);
	fscanf_s(fp, "%d", &map1.beginy);

	/* lit la val de tileset */

	fscanf_s(fp, "%d", &map1.tilesetAffiche);

	map1.maxX = map1.maxY = 0;

	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &map1.tile[y][x]);

			/* Permet de déterminer la taille de la map (voir plus bas) */
			if (map1.tile[y][x] > 0)
			{
				if (x > map1.maxX)
				{
					map1.maxX = x;
				}

				if (y > map1.maxY)
				{
					map1.maxY = y;
				}
			}
		}
	}

	//Deuxième couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &map1.tile2[y][x]);
		}
	}

	//Troisième couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &map1.tile3[y][x]);
		}
	}

	/* maxX et maxY sont les coordonnées de fin de la map1.
	On les trouve dès qu'il n'y a plus que des zéros à la suite.
	Comme ça, on peut faire des maps de tailles différentes avec la même
	structure de fichier. */
	map1.maxX = (map1.maxX + 1) * TILE_SIZE;
	map1.maxY = (map1.maxY + 1) * TILE_SIZE;

	/* Et on referme le fichier */
	fclose(fp);

}


void map::drawMap(int layer, plateforme *plateforme, Draw *draw, Init1 *init)
{
	;
	int x, y, mapX, x1, x2, mapY, y1, y2, xsource, ysource, a;

	/* On initialise mapX à la 1ère colonne qu'on doit blitter.
	Celle-ci correspond au x de la map (en pixels) divisés par la taille d'une tile (32)
	pour obtenir la bonne colonne de notre map
	Exemple : si x du début de la map = 1026, on fait 1026 / 32
	et on sait qu'on doit commencer par afficher la 32eme colonne de tiles de notre map */
	mapX = map1.startX / TILE_SIZE;

	/* Coordonnées de départ pour l'affichage de la map : permet
	de déterminer à quels coordonnées blitter la 1ère colonne de tiles au pixel près
	(par exemple, si la 1ère colonne n'est visible qu'en partie, on devra commencer à blitter
	hors écran, donc avoir des coordonnées négatives - d'où le -1). */
	x1 = (map1.startX % TILE_SIZE) * -1;

	/* Calcul des coordonnées de la fin de la map : jusqu'où doit-on blitter ?
	Logiquement, on doit aller à x1 (départ) + SCREEN_WIDTH (la largeur de l'écran).
	Mais si on a commencé à blitter en dehors de l'écran la première colonne, il
	va falloir rajouter une autre colonne de tiles sinon on va avoir des pixels
	blancs. C'est ce que fait : x1 == 0 ? 0 : TILE_SIZE qu'on pourrait traduire par:
	if(x1 != 0)
	x2 = x1 + SCREEN_WIDTH + TILE_SIZE , mais forcément, c'est plus long ;)*/
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	/* On fait exactement pareil pour calculer y */
	mapY = map1.startY / TILE_SIZE;
	y1 = (map1.startY % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);


	//On met en place un timer pour animer la map 
	if (map1.mapTimer <= 0)
	{
		if (map1.tileSetNumber == 0)
		{
			map1.tileSetNumber = 1;
			map1.mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
		else
		{
			map1.tileSetNumber = 0;
			map1.mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
	}
	else
		map1.mapTimer--;


	/* Dessine la carte en commençant par startX et startY */

	/* On dessine ligne par ligne en commençant par y1 (0) jusqu'à y2 (480)
	A chaque fois, on rajoute TILE_SIZE (donc 32), car on descend d'une ligne
	de tile (qui fait 32 pixels de hauteur) */
	if (layer == 1)
	{
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			/* A chaque début de ligne, on réinitialise mapX qui contient la colonne
			(0 au début puisqu'on ne scrolle pas) */
			mapX = map1.startX / TILE_SIZE;

			/* A chaque colonne de tile, on dessine la bonne tile en allant
			de x = 0 à x = 640 */
			for (x = x1; x < x2; x += TILE_SIZE)
			{

				//Si la tile à dessiner n'est pas une tile vide
				if (map1.tile[mapY][mapX] != 0)
				{

					/*On teste si c'est une tile plateforme flottante */
					if (map1.tile[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& map1.tile[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (map1.tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, map1.tile[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);

						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						map1.tile[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = map1.tile[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (map1.tileSetNumber == 0)
					draw->drawTile(map1.tileSet, x, y, xsource, ysource, init);
				else
					draw->drawTile(map1.tileSetB, x, y, xsource, ysource, init);

				mapX++;
			}

			mapY++;
		}
	}

	else if (layer == 2)
	{
		//Deuxième couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = map1.startX / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile à dessiner n'est pas une tile vide
				if (map1.tile2[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (map1.tile2[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& map1.tile2[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (map1.tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, map1.tile2[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						map1.tile2[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = map1.tile2[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (map1.tileSetNumber == 0)
					draw->drawTile(map1.tileSet, x, y, xsource, ysource, init);
				else
					draw->drawTile(map1.tileSetB, x, y, xsource, ysource, init);

				mapX++;
			}
			mapY++;
		}
	}

	else if (layer == 3)
	{
		//Troisième couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = map1.startX / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile à dessiner n'est pas une tile vide
				if (map1.tile3[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (map1.tile3[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& map1.tile3[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (map1.tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, map1.tile3[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						map1.tile3[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = map1.tile3[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (map1.tileSetNumber == 0)
					draw->drawTile(map1.tileSet, x, y, xsource, ysource, init);
				else
					draw->drawTile(map1.tileSetB, x, y, xsource, ysource, init);

				mapX++;
			}
			mapY++;
		}
	}


}


void map::changeLevel(Draw *draw, Joueur *joueur, Init1 *init)
{
	char file[200];

	/* Charge la map depuis le fichier */

	sprintf_s(file, sizeof(file), "map/map%d.txt", joueur->getLevel());
	loadMap(file);

	//Charge le tileset
	/*if (map1.tileSet != NULL)
	{
		SDL_DestroyTexture(map1.tileSet);
	}
	if (map1.tileSetB != NULL)
	{
		SDL_DestroyTexture(map1.tileSetB);
	}*/

	sprintf_s(file, sizeof(file), "graphics/tileset%d.png", map1.tilesetAffiche);
	map1.tileSet = draw->loadImage(file, init);

	sprintf_s(file, sizeof(file), "graphics/tileset%dB.png", map1.tilesetAffiche);
	map1.tileSetB = draw->loadImage(file, init);

}



void map::cleanMaps(void)
{
	;
	// Libère la texture du background 
	if (map1.background != NULL)
	{
		SDL_DestroyTexture(map1.background);
		map1.background = NULL;
	}

	// Libère les textures des tilesets 
	if (map1.tileSet != NULL)
	{
		SDL_DestroyTexture(map1.tileSet);
		map1.tileSet = NULL;
	}

	if (map1.tileSetB != NULL)
	{
		SDL_DestroyTexture(map1.tileSetB);
		map1.tileSetB = NULL;
	}

}



void map::mapCollision(GameObject *entity, Joueur *joueur, Son *son, plateforme *plateforme, Draw *draw, Init1 *init, map *map)
{
	int i, x1, x2, y1, y2;

	/* D'abord, on considère le joueur en l'air jusqu'à temps
	d'être sûr qu'il touche le sol */
	entity->onGround = 0;

	/* Ensuite, on va tester les mouvements horizontaux en premier
	(axe des X). On va se servir de i comme compteur pour notre boucle.
	En fait, on va découper notre sprite en blocs de tiles pour voir
	quelles tiles il est susceptible de recouvrir.
	On va donc commencer en donnant la valeur de Tile_Size à i pour qu'il
	teste la tile où se trouve le x du joueur mais aussi la suivante SAUF
	dans le cas où notre sprite serait inférieur à 	la taille d'une tile.
	Dans ce cas, on lui donnera la vraie valeur de la taille du sprite
	Et on testera ensuite 2 fois la même tile. Mais comme ça notre code
	sera opérationnel quelle que soit la taille de nos sprites ! */

	if (entity->h > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->h;


	//On lance alors une boucle for infinie car on l'interrompra selon 
	//les résultats de nos calculs
	for (;;)
	{
		//On va calculer ici les coins de notre sprite à gauche et à 
		//droite pour voir quelle tile ils touchent.
		x1 = (entity->x + entity->dirX) / TILE_SIZE;
		x2 = (entity->x + entity->dirX + entity->w - 1) / TILE_SIZE;

		//Même chose avec y, sauf qu'on va monter au fur et à mesure 
		//pour tester toute la hauteur de notre sprite, grâce à notre 
		//fameuse variable i.
		y1 = (entity->y) / TILE_SIZE;
		y2 = (entity->y + i - 1) / TILE_SIZE;

		//De là, on va tester les mouvements initiés dans updatePlayer 
		//grâce aux vecteurs dirX et dirY, tout en testant avant qu'on 
		//se situe bien dans les limites de l'écran.
		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			//Si on a un mouvement à droite
			if (entity->dirX > 0)
			{

				//Test des tiles Power-up
				if (map1.tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y1][x2] = 0;
				}
				else if (map1.tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y2][x2] = 0;
				}


				//Test de la tile checkpoint
				if (map1.tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x2 * TILE_SIZE;
					entity->respawnY = (y1 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y1][x2] += 1;
				}
				else if (map1.tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x2 * TILE_SIZE;
					entity->respawnY = (y2 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y2][x2] += 1;
				}


				//On vérifie si les tiles recouvertes sont solides
				if (map1.tile[y1][x2] > BLANK_TILE || map1.tile[y2][x2] > BLANK_TILE)
				{
					// Si c'est le cas, on place le joueur aussi près que possible
					// de ces tiles, en mettant à jour ses coordonnées. Enfin, on 
					//réinitialise son vecteur déplacement (dirX).

					entity->x = x2 * TILE_SIZE;
					entity->x -= entity->w + 1;
					entity->dirX = 0;

				}
			}

			//Même chose à gauche
			else if (entity->dirX < 0)
			{

				//Test des tiles Power-up : Etoile et vie
				if (map1.tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y1][x1] = 0;
				}
				else if (map1.tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y2][x1] = 0;
				}

				//Test de la tile checkpoint
				if (map1.tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x1 * TILE_SIZE;
					entity->respawnY = (y1 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y1][x1] += 1;
				}
				else if (map1.tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x1 * TILE_SIZE;
					entity->respawnY = (y2 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y2][x1] += 1;
				}


				if (map1.tile[y1][x1] > BLANK_TILE || map1.tile[y2][x1] > BLANK_TILE)
				{
					entity->x = (x1 + 1) * TILE_SIZE;
					entity->dirX = 0;
				}

			}

		}

		//On sort de la boucle si on a testé toutes les tiles le long de la hauteur du sprite.
		if (i == entity->h)
		{
			break;
		}

		//Sinon, on teste les tiles supérieures en se limitant à la heuteur du sprite.
		i += TILE_SIZE;

		if (i > entity->h)
		{
			i = entity->h;
		}
	}

	//On recommence la même chose avec le mouvement vertical (axe des Y)
	if (entity->w > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->w;


	for (;;)
	{
		x1 = (entity->x) / TILE_SIZE;
		x2 = (entity->x + i) / TILE_SIZE;

		y1 = (entity->y + entity->dirY) / TILE_SIZE;
		y2 = (entity->y + entity->dirY + entity->h) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			if (entity->dirY > 0)
			{
				/* Déplacement en bas */

				//Test des tiles Power-up)
				if (map1.tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y2][x1] = 0;
				}
				else if (map1.tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y2][x2] = 0;
				}

				//Test de la tile checkpoint
				if (map1.tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x1 * TILE_SIZE;
					entity->respawnY = (y2 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y2][x1] += 1;
				}
				else if (map1.tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x2 * TILE_SIZE;
					entity->respawnY = (y2 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y2][x2] += 1;
				}


				/* Gestion des pics */
				if ((map1.tile[y2][x1] == TILE_PIKES) || (map1.tile[y2][x2] == TILE_PIKES))
				{

					//On joue le son
					son->playSoundFx(1);
					//On fait sauter le joueur
					entity->dirY = -JUMP_HEIGHT;

					if (entity->life > 1)
					{
						//Si le timer d'invincibilité est à 0
						//on perd un coeur
						if (entity->invincibleTimer == 0)
						{
							entity->life--;
							entity->invincibleTimer = 80;
						}
					}
					else
					{
						//On met le timer à 1 pour tuer le joueur intantanément
						entity->timerMort = 1;
						//On joue le son
						son->playSoundFx(1);
					}
				}

				/* Gestion du ressort */
				else if ((map1.tile[y2][x1] == TILE_RESSORT) || (map1.tile[y2][x2] == TILE_RESSORT))
				{
					entity->dirY = -20;
					//On indique au jeu qu'il a atterri pour réinitialiser le double saut
					entity->onGround = 1;
					son->playSoundFx(0);
				}


				//Gestion des plateformes traversables : elles se situent juste avant 
				//les tiles bloquantes dans notre tileset (dont la valeur butoire est 
				//BLANK_TILE). Il suffit donc d'utiliser le numéro de la première tile 
				//traversable au lieu de BLANK_TILE pour bloquer le joueur, 
				//seulement quand il tombe dessus (sinon, il passe au-travers
				//et le test n'est donc pas effectué dans les autres directions 
				else if (map1.tile[y2][x1] > TILE_TRAVERSABLE || map1.tile[y2][x2] > TILE_TRAVERSABLE)
				{
					//Si la tile est une plateforme ou une tile solide, on y colle le joueur et
					//on le déclare sur le sol (onGround).
					entity->y = y2 * TILE_SIZE;
					entity->y -= entity->h;
					entity->dirY = 0;
					entity->onGround = 1;
				}


				// Test de collision avec la plateforme mobile 
				//if (plateforme->getPlateformeNumber > 0)
					//plateforme->checkCollisionsWithPlateforms(entity);

			}

			else if (entity->dirY < 0)
			{

				/* Déplacement vers le haut */

				//Test des tiles Power-up 
				if (map1.tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y1][x1] = 0;
				}
				if (map1.tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& map1.tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					joueur->getItem(map1.tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, son);

					//On remplace la tile power-up par une tile transparente
					map1.tile[y1][x2] = 0;
				}

				//Test de la tile checkpoint
				if (map1.tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x1 * TILE_SIZE;
					entity->respawnY = (y1 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y1][x1] += 1;
				}
				else if (map1.tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->checkpointActif = 1;

					//On enregistre les coordonnées
					entity->respawnX = x2 * TILE_SIZE;
					entity->respawnY = (y1 * TILE_SIZE) - entity->h;

					//On change la tile
					map1.tile[y1][x2] += 1;
				}


				if (map1.tile[y1][x1] > BLANK_TILE || map1.tile[y1][x2] > BLANK_TILE)
				{
					entity->y = (y1 + 1) * TILE_SIZE;
					entity->dirY = 0;
				}

			}
		}

		//On teste la largeur du sprite (même technique que pour la hauteur précédemment)
		if (i == entity->w)
		{
			break;
		}

		i += TILE_SIZE;

		if (i > entity->w)
		{
			i = entity->w;
		}
	}

	/* Maintenant, on applique les vecteurs de mouvement si le sprite n'est pas bloqué */
	entity->x += entity->dirX;
	entity->y += entity->dirY;

	//Et on contraint son déplacement aux limites de l'écran.
	if (entity->x < 0)
	{
		entity->x = 0;
	}

	else if (entity->x + entity->w >= map1.maxX)
	{
		//Si on touche le bord droit de l'écran, on passe au niveau sup
		joueur->SetValeurDuNiveau(joueur->getLevel() + 1);
		//Si on dépasse le niveau max, on annule et on limite le déplacement du joueur
		if (joueur->getLevel() > LEVEL_MAX)
		{
			joueur->SetValeurDuNiveau(LEVEL_MAX);
			entity->x = map1.maxX - entity->w - 1;
		}
		//Sinon, on passe au niveau sup, on charge la nouvelle map et on réinitialise le joueur
		else
		{
			//On désactive le checkpoint
			entity->checkpointActif = 0;

			changeLevel(draw, joueur, init);
			joueur->initializePlayer(1, plateforme, map);
		}
	}

	//Maintenant, s'il sort de l'écran par le bas (chute dans un trou sans fond), on lance le timer
	//qui gère sa mort et sa réinitialisation (plus tard on gèrera aussi les vies).
	if (entity->y > map1.maxY)
	{
		entity->timerMort = 60;
	}
}
