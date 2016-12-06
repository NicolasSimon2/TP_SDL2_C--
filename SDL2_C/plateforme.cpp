#include "plateforme.h"
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
#include "map.h"
#include "menu.h"
#include "Plateforme.h"


plateforme::plateforme()
{
}


plateforme::~plateforme()
{
}




void plateforme::loadPlateforme(Draw *draw, Init1 *init)
{
	//On charge l'image de la plateforme
	plateformeTexture = draw->loadImage("graphics/plateforme.png", init);

	/* Si on ne peut pas, on quitte, et on l'indique en erreur ;) */
	if (plateformeTexture == NULL)
	{
		printf("Impossible de charger l'image de la plateforme : graphics/plateforme.png/n");
		exit(1);
	}
}


void plateforme::cleanPlateforme(void)
{
	if (plateformeTexture != NULL)
	{
		SDL_DestroyTexture(plateformeTexture);
	}
}


int plateforme::getPlateformeNumber(void)
{
	return nombrePlateformes;
}


void plateforme::resetPlateformes(void)
{
	nombrePlateformes = 0;
}


void plateforme::initPlateforme(int x, int y, int type)
{

	/* On rajoute une plateforme à la liste des plateformes */
	nombrePlateformes++;

	//S'il y en a trop, on fait une erreur et on quitte
	if (nombrePlateformes > PLATEFORMES_MAX)
	{
		printf("Trop de plateformes initialisees !\nEnlevez-en ou changez leur nombre max.\n");
		exit(1);
	}

	//On remplit notre structure
	plateforme1[nombrePlateformes].x = plateforme1[nombrePlateformes].beginX = x;
	plateforme1[nombrePlateformes].y = plateforme1[nombrePlateformes].beginY = y;

	//On enregistre la taille de l'image
	SDL_QueryTexture(plateformeTexture, NULL, NULL, &plateforme1[nombrePlateformes].w, &plateforme1[nombrePlateformes].h);

	plateforme1[nombrePlateformes].type = type;

	//Si elle est du type 2, elle monte, sinon elle va à droite
	if (plateforme1[nombrePlateformes].type == 2)
		plateforme1[nombrePlateformes].direction = UP;
	else
		plateforme1[nombrePlateformes].direction = RIGHT;

}


void plateforme::doPlateforme(Joueur *joueur)
{

	int i;

	for (i = 1; i <= nombrePlateformes; i++)
	{
		/* Déplacement UP/DOWN (haut/bas) */
		if (plateforme1[i].type == 2)
		{
			if (plateforme1[i].direction == UP)
			{
				plateforme1[i].y -= PLATEFORM_SPEED;
				/* Si le joueur est dessus, on le déplace avec
				pour éviter qu'il ne reste immobile et que la
				plateforme se barre comme sur certains vieux
				(mauvais) jeux...*/
				if (plateforme1[i].player_dessus == 1)
					joueur->setPlayery(joueur->getPlayery() - PLATEFORM_SPEED);
			}
			else
			{
				plateforme1[i].y += PLATEFORM_SPEED;
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme1[i].player_dessus == 1)
					joueur->setPlayery(joueur->getPlayery() + PLATEFORM_SPEED);
			}

			if (plateforme1[i].y > plateforme1[i].beginY + 5 * TILE_SIZE)
				plateforme1[i].direction = UP;


			if (plateforme1[i].y < plateforme1[i].beginY)
				plateforme1[i].direction = DOWN;
		}

		/* Déplacement RIGHT/LEFT */
		else
		{
			if (plateforme1[i].direction == RIGHT)
			{
				plateforme1[i].x += PLATEFORM_SPEED;
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme1[i].player_dessus == 1)
					joueur->setPlayerx(joueur->getPlayerx() + PLATEFORM_SPEED);
			}
			else
			{
				plateforme1[i].x -= PLATEFORM_SPEED;
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme1[i].player_dessus == 1)
					joueur->setPlayerx(joueur->getPlayerx() - PLATEFORM_SPEED);
			}

			//Test : si la plateforme dépasse 5 tiles de longueur,
			//on lui fait faire demi-tour pour ne pas qu'elle
			//fasse toute la map ! :D
			if (plateforme1[i].x > plateforme1[i].beginX + 5 * TILE_SIZE)
				plateforme1[i].direction = LEFT;

			if (plateforme1[i].x < plateforme1[i].beginX)
				plateforme1[i].direction = RIGHT;
		}

	}

}


void plateforme::drawPlateforme(map *map, Draw *draw, Init1 *init)
{

	int i;

	for (i = 1; i <= nombrePlateformes; i++)
	{
		draw->drawImage(plateformeTexture, plateforme1[i].x - map->getStartX(), plateforme1[i].y - map->getStartY(), init);
	}

}



void plateforme::checkCollisionsWithPlateforms(GameObject *entity)
{
	int j;

	for (j = 1; j <= nombrePlateformes; j++)
	{
		if (entity->x + entity->w >= plateforme1[j].x
			&& entity->x <= plateforme1[j].x + plateforme1[j].w
			&& entity->y + entity->h >= plateforme1[j].y
			&& entity->y + entity->h < plateforme1[j].y + 32)
		{
			// Place the player as close to the plateform as possible 
			entity->y = plateforme1[j].y - entity->h;
			entity->dirY = 0;
			entity->onGround = 1;

			plateforme1[j].player_dessus = 1;
		}
		else
			plateforme1[j].player_dessus = 0;
	}

}

