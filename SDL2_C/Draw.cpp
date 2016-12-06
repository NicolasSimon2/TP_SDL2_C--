#include "Draw.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "input.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"


Draw::Draw()
{
}


Draw::~Draw()
{
}

//Rabidja 3 - nouvelle version intégralement en SDL 2.0 
//Modification : Nicolas Lehmann











void Draw::drawGame(int pauseMenu, map *map, Joueur *joueur, plateforme *plateforme, menu *menu, Init1 *init, Draw *draw, Font *font)
{


	// Affiche le fond (background) aux coordonnées (0,0) 
	drawImage(map->getBackground(), 0, 0, init);

	/* Affiche la map de tiles : layer 2 (couche du fond) */
	map->drawMap(2, plateforme, draw, init);


	/* Affiche la map de tiles : layer 1 (couche active : sol, etc.)*/
	map->drawMap(1, plateforme, draw, init);

	/* Affiche le joueur */
	joueur->drawPlayer(map, init);


	//Affiche les plateformes flottantes
	plateforme->drawPlateforme(map, draw, init);


	/* Affiche la map de tiles : layer 3 (couche en foreground / devant) */
	map->drawMap(3, plateforme, draw, init);

	//On affiche le HUD par-dessus tout le reste
	drawHud(joueur, map, font, init);

	//On affiche le menu Pause, si on est en Pause
	if (pauseMenu)
		menu->drawPauseMenu(init, font);

	// Affiche l'écran 
	SDL_RenderPresent(init->getrenderer());

	// Délai pour laisser respirer le proc 
	SDL_Delay(1);
}



SDL_Texture *Draw::loadImage(char *name, Init1 *init)
{
	/* Charge les images avec SDL Image dans une SDL_Surface */

	SDL_Surface *loadedImage = NULL;
	SDL_Texture *texture = NULL;
	loadedImage = IMG_Load(name);

	if (loadedImage != NULL)
	{
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(init->getrenderer(), loadedImage);

		// On se débarrasse du pointeur vers une surface
		SDL_FreeSurface(loadedImage);
		loadedImage = NULL;
	}
	else
		fprintf(stderr, "L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

	return texture;
}



void Draw::drawImage(SDL_Texture *image, int x, int y, Init1 *init)
{
	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(init->getrenderer(), image, NULL, &dest);
}


void Draw::drawImagePlus(SDL_Texture *image, int x, int y, double rotation, SDL_Point center, SDL_RendererFlip flip, Init1 *init)
{
	//Nouvelle fonction en SDL2 qui gère le flip et les rotations

	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopyEx(init->getrenderer(), image, NULL, &dest, rotation, &center, flip);
}



void Draw::drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy, Init1 *init)
{
	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	/* Rectangle source */
	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(init->getrenderer(), image, &src, &dest);
}



void Draw::delay(unsigned int frameLimit)
{
	// Gestion des 60 fps (images/stories/seconde)
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}

	if (frameLimit > ticks + 16)
	{
		SDL_Delay(16);
	}

	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}



void Draw::drawHud(Joueur *joueur, map *map, Font *font, Init1 *init)
{
	//On crée une varuiable qui contiendra notre texte (jusqu'à 200 caractères, y'a de la marge ;) ).
	char text[200];

	int i;

	//Affiche le nombre de coeurs
	//On crée une boucle pour afficher de 1 à 3 coeurs
	//selon la vie, avec un décalage de 32 pixels
	for (i = 0; i < joueur->getLife(); i++)
	{
		// Calcul pour découper le tileset comme dans la fonction drawMap()
		int ysource = TILE_POWER_UP_COEUR / 10 * TILE_SIZE;
		int xsource = TILE_POWER_UP_COEUR % 10 * TILE_SIZE;

		drawTile(map->getTileSetA(), 60 + i * 32, 20, xsource, ysource, init);
	}

	/* Affiche le nombre de vies en bas à droite - Adaptation à la fenêtre auto */
	drawImage(HUD_vie, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 70, init);

	//Pour afficher le nombre de vies, on formate notre string pour qu'il prenne la valeur de la variable
	//sprintf(text, "x %d", getNombreDeVies());   //Version dépréciée remplacée par :
	sprintf_s(text, sizeof(text), "x %d", joueur->getNombreDeVies());


	//Puis on utilise notre fonction créée précédemment pour écrire en noir (0, 0, 0, 255)
	//et en blanc (255, 255, 255, 255) afin de surligner le texte et de le rendre plus
	//visible
	font->drawString(text, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 60, 0, 0, 0, 255, init);
	font->drawString(text, SCREEN_WIDTH - 82, SCREEN_HEIGHT - 62, 255, 255, 255, 255, init);

	/* Affiche le nombre d'étoiles en haut à gauche */
	drawImage(HUD_etoiles, 60, 60, init);

	sprintf_s(text, sizeof(text), "%d", joueur->getNombreDetoiles());
	font->drawString(text, 100, 57, 0, 0, 0, 255, init);
	font->drawString(text, 98, 55, 255, 255, 255, 255, init);

}


void Draw::initHUD(Init1 *init)
{
	/* On charge les images du HUD */
	HUD_vie = loadImage("graphics/life.png", init);
	HUD_etoiles = loadImage("graphics/stars.png", init);
}


void Draw::cleanHUD(void)
{
	if (HUD_etoiles != NULL)
	{
		SDL_DestroyTexture(HUD_etoiles);
	}
	if (HUD_vie != NULL)
	{
		SDL_DestroyTexture(HUD_vie);
	}
}