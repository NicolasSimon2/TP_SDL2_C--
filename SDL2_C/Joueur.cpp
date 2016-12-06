#include "Joueur.h"
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


Joueur::Joueur()
{
}


Joueur::~Joueur()
{
}

void Joueur::getPlayer(GameObject gameobject)
{

	gameobject = this->player;
}





int Joueur::getPlayerDirection(void)
{
	return player.direction;
}


void Joueur::resetCheckpoint(void)
{
	player.checkpointActif = 0;
}


//Renvoie les coordonnées x du héros
int Joueur::getPlayerx(void)
{
	return player.x;
}


//Renvoie les coordonnées y du héros
int Joueur::getPlayery(void)
{
	return player.y;
}


//Change la valeur des coordonnées x du héros
void Joueur::setPlayerx(int valeur)
{
	player.x = valeur;
}


//Change la valeur des coordonnées y du héros
void Joueur::setPlayery(int valeur)
{
	player.y = valeur;
}


//Renvoie le numéro du niveau en cours
int Joueur::getLevel(void)
{
	return level;
}


//Change la valeur du niveau en cours
void Joueur::SetValeurDuNiveau(int valeur)
{
	level = valeur;
}


int Joueur::getLife(void)
{
	return player.life;
}


int Joueur::getNombreDeVies(void)
{
	return vies;
}

void Joueur::setNombreDeVies(int valeur)
{
	vies = valeur;
}

int Joueur::getNombreDetoiles(void)
{
	return etoiles;
}

void Joueur::setNombreDetoiles(int valeur)
{
	etoiles = valeur;
}


//Charge la spritesheet (=feuille de sprites) de notre héros
//au début du jeu
void Joueur::initPlayerSprites(Draw *draw, Init1 *init)
{
	playerSpriteSheet = draw->loadImage("graphics/rabidja.png", init);
}


//Libère le sprite du héros à la fin du jeu
void Joueur::cleanPlayer(void)
{
	if (playerSpriteSheet != NULL)
	{
		SDL_DestroyTexture(playerSpriteSheet);
		playerSpriteSheet = NULL;
	}
}




void Joueur::killPlayer(Son *son)
{
	//On met le timer à 1 pour tuer le joueur intantanément
	player.timerMort = 1;
	//On joue le son
	son->playSoundFx(1);
}

/*void Joueur::playerHurts(GameObject *monster, Son *son)
{
	//Si le timer d'invincibilité est à 0
	//on perd un coeur
	if (player.invincibleTimer == 0)
	{
		player.life--;
		player.invincibleTimer = 80;
		//On joue le son
		son.playSoundFx(1);
		monster->timerMort = 1;
		player.dirY = -JUMP_HEIGHT;
	}
}*/



void Joueur::initializePlayer(int newLevel, plateforme *plateforme, map *map)
{

	//PV à 3
	player.life = 3;

	//Timer d'invincibilité à 0
	player.invincibleTimer = 0;


	//Nombre de plateformes flottantes à 0
	plateforme->resetPlateformes();

	//Indique l'état et la direction de notre héros
	player.direction = RIGHT;
	player.etat = IDLE;

	//Indique le numéro de la frame où commencer
	player.frameNumber = 0;
	//...la valeur de son chrono ou timer
	player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
	//... et son nombre de frames max (8 pour l'anim' IDLE
	// = ne fait rien)
	player.frameMax = 8;

	/* Coordonnées de démarrage/respawn de notre héros */
	if (player.checkpointActif == 1)
	{
		player.x = player.respawnX;
		player.y = player.respawnY;
	}
	else
	{
		player.x = map->getBeginX();
		player.y = map->getBeginY();
	}

	//On réinitiliase les coordonnées de la caméra 
	//si on change de niveau
	if (newLevel == 1)
	{
		map->setStartX(map->getBeginX());
		map->setStartY(map->getBeginY());
	}

	/* Hauteur et largeur de notre héros */
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGTH;

	//Variables nécessaires au fonctionnement de la gestion des collisions
	player.timerMort = 0;
	player.onGround = 0;




}


void Joueur::updatePlayer(Input *input, Son *son, menu *menu, map *map, Joueur *joueur, plateforme *plateforme, Draw *draw, Init1 *init)
{
	//On rajoute un timer au cas où notre héros mourrait lamentablement en tombant dans un trou...
	//Si le timer vaut 0, c'est que tout va bien, sinon, on le décrémente jusqu'à 0, et là,
	//on réinitialise.
	//C'est pour ça qu'on ne gère le joueur que si ce timer vaut 0.
	if (player.timerMort == 0)
	{
		//On gère le timer de l'invincibilité
		if (player.invincibleTimer > 0)
			player.invincibleTimer--;

		//On réinitialise notre vecteur de déplacement latéral (X), pour éviter que le perso
		//ne fonce de plus en plus vite pour atteindre la vitesse de la lumière ! ;)
		//Essayez de le désactiver pour voir !
		player.dirX = 0;

		// La gravité fait toujours tomber le perso : on incrémente donc le vecteur Y
		player.dirY += GRAVITY_SPEED;

		//Mais on le limite pour ne pas que le joueur se mette à tomber trop vite quand même
		if (player.dirY >= MAX_FALL_SPEED)
		{
			player.dirY = MAX_FALL_SPEED;
		}

		//Voilà, au lieu de changer directement les coordonnées du joueur, on passe par un vecteur
		//qui sera utilisé par la fonction mapCollision(), qui regardera si on peut ou pas déplacer
		//le joueur selon ce vecteur et changera les coordonnées du player en fonction.
		if (input->getLeft() == 1)
		{
			player.dirX -= PLAYER_SPEED;
			//Et on indique qu'il va à gauche (pour le flip
			//de l'affichage, rappelez-vous).
			player.direction = LEFT;

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (player.etat != WALK && player.onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				player.etat = WALK;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}
		}

		//Si on détecte un appui sur la touche fléchée droite
		else if (input->getRight() == 1)
		{
			//On augmente les coordonnées en x du joueur
			player.dirX += PLAYER_SPEED;
			//Et on indique qu'il va à droite (pour le flip
			//de l'affichage, rappelez-vous).
			player.direction = RIGHT;

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (player.etat != WALK && player.onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				player.etat = WALK;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}
		}

		//Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivité (Idle)
		else if (input->getRight() == 0 && input->getLeft() == 0 && player.onGround == 1)
		{
			//On teste si le joueur n'était pas déjà inactif, pour ne pas recharger l'animation
			//à chaque tour de boucle
			if (player.etat != IDLE)
			{
				//On enregistre l'anim' de l'inactivité et on l'initialise à 0
				player.etat = IDLE;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}

		}


		//Et voici la fonction de saut très simple :
		//Si on appuie sur la touche saut et qu'on est sur le sol, alors on attribue une valeur
		//négative au vecteur Y
		//parce que sauter veut dire se rapprocher du haut de l'écran et donc de y=0.
		if (input->getC() == 1)
		{
			if (player.onGround == 1)
			{
				player.dirY = -JUMP_HEIGHT;
				player.onGround = 0;
				player.jump = 1;
				son->playSoundFx(2);
			}
			/* Si on est en saut 1, on peut faire un deuxième bond et on remet jump1 à 0 */
			else if (player.jump == 1)
			{
				player.dirY = -JUMP_HEIGHT;
				player.jump = 0;
				son->playSoundFx(2);
			}
			input->setCZ();
		}

		//Si on appuie sur Enter
		if (input->getEnter() == 1)
		{
			//On met le jeu en pause
			menu->setOnMenu(1, 1);
			input->setEnterZ();
		}



		/* Réactive la possibilité de double saut si on tombe sans sauter */
		if (player.onGround == 1)
			player.jump = 1;


		//On gère l'anim du saut
		if (player.onGround == 0)
		{
			//Si on est en saut 1, on met l'anim' du saut normal
			if (player.jump == 1)
			{
				if (player.etat != JUMP1)
				{
					player.etat = JUMP1;
					player.frameNumber = 0;
					player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
					player.frameMax = 2;
				}
			}
			else
			{
				if (player.etat != JUMP2)
				{
					player.etat = JUMP2;
					player.frameNumber = 0;
					player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
					player.frameMax = 4;
				}
			}
		}

		//On rajoute notre fonction de détection des collisions qui va mettre à 
		//jour les coordonnées de notre super lapin.
		//map->mapCollision(player, joueur, son, plateforme, draw, init, map);

		//On gère le scrolling (fonction ci-dessous)
		centerScrollingOnPlayer(map);

	}

	//Gestion de la mort quand le héros tombe dans un trou :
	//Si timerMort est différent de 0, c'est qu'il faut réinitialiser le joueur.
	//On ignore alors ce qui précède et on joue cette boucle (un wait en fait) jusqu'à ce que
	// timerMort == 1. A ce moment-là, on le décrémente encore -> il vaut 0 et on réinitialise
	//le jeu avec notre bonne vieille fonction d'initialisation ;) !
	if (player.timerMort > 0)
	{
		player.timerMort--;

		if (player.timerMort == 0)
		{
			// Si on est mort, on perd une vie 
			setNombreDeVies(getNombreDeVies() - 1);

			//Sauf si on a plus de vies...
			if (getNombreDeVies() < 0)
			{
				//Dans ce cas on retourne au menu start
				menu->setOnMenu(1, 0);
			}

			//Sinon on réinitialise le niveau
			map->changeLevel(draw, joueur, init);
			initializePlayer(1, plateforme, map);

		}
	}
}



void Joueur::drawPlayer(map *map, Init1 *init)
{
	/* Gestion du timer */
	// Si notre timer (un compte à rebours en fait) arrive à zéro
	if (player.frameTimer <= 0)
	{
		//On le réinitialise
		player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//Et on incrémente notre variable qui compte les frames de 1 pour passer à la suivante
		player.frameNumber++;

		//Mais si on dépasse la frame max, il faut revenir à la première :
		if (player.frameNumber >= player.frameMax)
			player.frameNumber = 0;

	}
	//Sinon, on décrémente notre timer
	else
		player.frameTimer--;


	//Ensuite, on peut passer la main à notre fonction

	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	// On soustrait des coordonnées de notre héros, ceux du début de la map, pour qu'il colle
	//au scrolling :
	dest.x = player.x - map->getStartX();
	dest.y = player.y - map->getStartY();
	dest.w = player.w;
	dest.h = player.h;

	/* Rectangle source */
	SDL_Rect src;

	//Pour connaître le X de la bonne frame à dessiner, il suffit de multiplier
	//la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = player.frameNumber * player.w;
	src.w = player.w;
	src.h = player.h;

	//On calcule le Y de la bonne frame à dessiner, selon la valeur de l'état du héros :
	//Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
	//Tout cela en accord avec notre spritesheet, of course ;)
	src.y = player.etat * player.h;

	//Si on a été touché et qu'on est invincible
	if (player.invincibleTimer > 0)
	{
		//On fait clignoter le héros une frame sur deux
		//Pour ça, on calcule si le numéro de la frame en 
		//cours est un multiple de deux
		if (player.frameNumber % 2 == 0)
		{
			//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
			if (player.direction == LEFT)
				SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
			else
				SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
		}
		//Sinon, on ne dessine rien, pour le faire clignoter
	}

	//Sinon, on dessine normalement
	else
	{
		//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
		if (player.direction == LEFT)
			SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
	}

}




void Joueur::centerScrollingOnPlayer(map *map)
{
	// Nouveau scrolling à sous-boîte limite :
	//Pour éviter les effets de saccades dus à une caméra qui se 
	//centre automatiquement et constamment sur le joueur (ce qui 
	//peut en rendre malade certains...), on crée une "boîte" imaginaire 
	//autour du joueur. Quand on dépasse un de ses bords (en haut, en bas,
	//à gauche ou à droite), on scrolle.
	//Mais là encore, au lieu de centrer sur le joueur, on déplace simplement
	//la caméra jusqu'à arriver au joueur. On a changé ici la valeur à 4 pixels 
	//pour que le jeu soit plus rapide. 
	int cxperso = player.x + player.w / 2;
	int cyperso = player.y + player.h / 2;
	int xlimmin = map->getStartX() + LIMITE_X;
	int xlimmax = xlimmin + LIMITE_W;
	int ylimmin = map->getStartY() + LIMITE_Y;
	int ylimmax = ylimmin + LIMITE_H;

	//Effet de retour en arrière quand on est mort :
	//Si on est très loin de la caméra, plus loin que le bord
	//de la map, on accélère le scrolling :
	if (cxperso < map->getStartX())
	{
		map->setStartX(map->getStartX() - 30);
	}

	//Si on dépasse par la gauche, on recule la caméra
	else if (cxperso < xlimmin)
	{
		map->setStartX(map->getStartX() - 4);
	}

	//Effet de retour en avant quand on est mort (au
	//cas où le joueur s'amuse à faire le niveau à rebours
	//après une checkpoint) :
	//Si on est très loin de la caméra, plus loin que le bord
	//de la map, on accélère le scrolling :
	if (cxperso > map->getStartX() + SCREEN_WIDTH)
	{
		map->setStartX(map->getStartX() + 30);
	}

	//Si on dépasse par la droite, on avance la caméra
	else if (cxperso > xlimmax)
	{
		map->setStartX(map->getStartX() + 4);
	}

	//Si on arrive au bout de la map à gauche, on stoppe le scrolling
	if (map->getStartX() < 0)
	{
		map->setStartX(0);
	}

	//Si on arrive au bout de la map à droite, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir).
	else if (map->getStartX() + SCREEN_WIDTH >= map->getMaxX())
	{
		map->setStartX(map->getMaxX() - SCREEN_WIDTH);
	}

	//Si on dépasse par le haut, on remonte la caméra 
	if (cyperso < ylimmin)
	{
		map->setStartY(map->getStartY() - 4);
	}

	//Si on dépasse par le bas, on descend la caméra
	if (cyperso > ylimmax)
	{
		//Sauf si on tombe très vite, auquel cas, on accélère la caméra :
		if (player.dirY >= MAX_FALL_SPEED - 2)
		{
			map->setStartY(map->getStartY() + MAX_FALL_SPEED + 1);
		}
		else
		{
			map->setStartY(map->getStartY() + 4);
		}
	}

	//Si on arrive au bout de la map en haut, on stoppe le scrolling
	if (map->getStartY() < 0)
	{
		map->setStartY(0);
	}

	//Si on arrive au bout de la map en bas, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir ;) ).
	else if (map->getStartY() + SCREEN_HEIGHT >= map->getMaxY())
	{
		map->setStartY(map->getMaxY() - SCREEN_HEIGHT);
	}

}



void Joueur::getItem(int itemNumber, Son *son)
{
	switch (itemNumber)
	{
		//Gestion des étoiles
	case 1:
		//On incrémente le compteur Etoile
		setNombreDetoiles(getNombreDetoiles() + 1);
		son->playSoundFx(3);

		//On teste s'il y a 100 étoiles : on remet le compteur à 0 et on rajoute une vie ;)
		if (getNombreDetoiles() >= 100)
		{
			setNombreDetoiles(0);
			//On incrémente le nombre de vies (max 99)
			if (getNombreDeVies() < 99)
				setNombreDeVies(getNombreDeVies() + 1);
		}
		break;

		//Gestion des coeurs
	case 2:
		//On incrémente le compteur Etoile
		if (player.life < 3)
			player.life++;

		son->playSoundFx(3);
		break;

		//Gestion des vies
	case 3:
		//On incrémente le nombre de vies (max 99)
		if (getNombreDeVies() < 99)
			setNombreDeVies(getNombreDeVies() + 1);

		son->playSoundFx(3);
		break;

	default:
		break;
	}


}