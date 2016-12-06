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


//Renvoie les coordonn�es x du h�ros
int Joueur::getPlayerx(void)
{
	return player.x;
}


//Renvoie les coordonn�es y du h�ros
int Joueur::getPlayery(void)
{
	return player.y;
}


//Change la valeur des coordonn�es x du h�ros
void Joueur::setPlayerx(int valeur)
{
	player.x = valeur;
}


//Change la valeur des coordonn�es y du h�ros
void Joueur::setPlayery(int valeur)
{
	player.y = valeur;
}


//Renvoie le num�ro du niveau en cours
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


//Charge la spritesheet (=feuille de sprites) de notre h�ros
//au d�but du jeu
void Joueur::initPlayerSprites(Draw *draw, Init1 *init)
{
	playerSpriteSheet = draw->loadImage("graphics/rabidja.png", init);
}


//Lib�re le sprite du h�ros � la fin du jeu
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
	//On met le timer � 1 pour tuer le joueur intantan�ment
	player.timerMort = 1;
	//On joue le son
	son->playSoundFx(1);
}

/*void Joueur::playerHurts(GameObject *monster, Son *son)
{
	//Si le timer d'invincibilit� est � 0
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

	//PV � 3
	player.life = 3;

	//Timer d'invincibilit� � 0
	player.invincibleTimer = 0;


	//Nombre de plateformes flottantes � 0
	plateforme->resetPlateformes();

	//Indique l'�tat et la direction de notre h�ros
	player.direction = RIGHT;
	player.etat = IDLE;

	//Indique le num�ro de la frame o� commencer
	player.frameNumber = 0;
	//...la valeur de son chrono ou timer
	player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
	//... et son nombre de frames max (8 pour l'anim' IDLE
	// = ne fait rien)
	player.frameMax = 8;

	/* Coordonn�es de d�marrage/respawn de notre h�ros */
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

	//On r�initiliase les coordonn�es de la cam�ra 
	//si on change de niveau
	if (newLevel == 1)
	{
		map->setStartX(map->getBeginX());
		map->setStartY(map->getBeginY());
	}

	/* Hauteur et largeur de notre h�ros */
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGTH;

	//Variables n�cessaires au fonctionnement de la gestion des collisions
	player.timerMort = 0;
	player.onGround = 0;




}


void Joueur::updatePlayer(Input *input, Son *son, menu *menu, map *map, Joueur *joueur, plateforme *plateforme, Draw *draw, Init1 *init)
{
	//On rajoute un timer au cas o� notre h�ros mourrait lamentablement en tombant dans un trou...
	//Si le timer vaut 0, c'est que tout va bien, sinon, on le d�cr�mente jusqu'� 0, et l�,
	//on r�initialise.
	//C'est pour �a qu'on ne g�re le joueur que si ce timer vaut 0.
	if (player.timerMort == 0)
	{
		//On g�re le timer de l'invincibilit�
		if (player.invincibleTimer > 0)
			player.invincibleTimer--;

		//On r�initialise notre vecteur de d�placement lat�ral (X), pour �viter que le perso
		//ne fonce de plus en plus vite pour atteindre la vitesse de la lumi�re ! ;)
		//Essayez de le d�sactiver pour voir !
		player.dirX = 0;

		// La gravit� fait toujours tomber le perso : on incr�mente donc le vecteur Y
		player.dirY += GRAVITY_SPEED;

		//Mais on le limite pour ne pas que le joueur se mette � tomber trop vite quand m�me
		if (player.dirY >= MAX_FALL_SPEED)
		{
			player.dirY = MAX_FALL_SPEED;
		}

		//Voil�, au lieu de changer directement les coordonn�es du joueur, on passe par un vecteur
		//qui sera utilis� par la fonction mapCollision(), qui regardera si on peut ou pas d�placer
		//le joueur selon ce vecteur et changera les coordonn�es du player en fonction.
		if (input->getLeft() == 1)
		{
			player.dirX -= PLAYER_SPEED;
			//Et on indique qu'il va � gauche (pour le flip
			//de l'affichage, rappelez-vous).
			player.direction = LEFT;

			//Si ce n'�tait pas son �tat auparavant et qu'il est bien sur
			//le sol (car l'anim' sera diff�rente s'il est en l'air)
			if (player.etat != WALK && player.onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise � 0
				player.etat = WALK;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}
		}

		//Si on d�tecte un appui sur la touche fl�ch�e droite
		else if (input->getRight() == 1)
		{
			//On augmente les coordonn�es en x du joueur
			player.dirX += PLAYER_SPEED;
			//Et on indique qu'il va � droite (pour le flip
			//de l'affichage, rappelez-vous).
			player.direction = RIGHT;

			//Si ce n'�tait pas son �tat auparavant et qu'il est bien sur
			//le sol (car l'anim' sera diff�rente s'il est en l'air)
			if (player.etat != WALK && player.onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise � 0
				player.etat = WALK;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}
		}

		//Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivit� (Idle)
		else if (input->getRight() == 0 && input->getLeft() == 0 && player.onGround == 1)
		{
			//On teste si le joueur n'�tait pas d�j� inactif, pour ne pas recharger l'animation
			//� chaque tour de boucle
			if (player.etat != IDLE)
			{
				//On enregistre l'anim' de l'inactivit� et on l'initialise � 0
				player.etat = IDLE;
				player.frameNumber = 0;
				player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				player.frameMax = 8;
			}

		}


		//Et voici la fonction de saut tr�s simple :
		//Si on appuie sur la touche saut et qu'on est sur le sol, alors on attribue une valeur
		//n�gative au vecteur Y
		//parce que sauter veut dire se rapprocher du haut de l'�cran et donc de y=0.
		if (input->getC() == 1)
		{
			if (player.onGround == 1)
			{
				player.dirY = -JUMP_HEIGHT;
				player.onGround = 0;
				player.jump = 1;
				son->playSoundFx(2);
			}
			/* Si on est en saut 1, on peut faire un deuxi�me bond et on remet jump1 � 0 */
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



		/* R�active la possibilit� de double saut si on tombe sans sauter */
		if (player.onGround == 1)
			player.jump = 1;


		//On g�re l'anim du saut
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

		//On rajoute notre fonction de d�tection des collisions qui va mettre � 
		//jour les coordonn�es de notre super lapin.
		//map->mapCollision(player, joueur, son, plateforme, draw, init, map);

		//On g�re le scrolling (fonction ci-dessous)
		centerScrollingOnPlayer(map);

	}

	//Gestion de la mort quand le h�ros tombe dans un trou :
	//Si timerMort est diff�rent de 0, c'est qu'il faut r�initialiser le joueur.
	//On ignore alors ce qui pr�c�de et on joue cette boucle (un wait en fait) jusqu'� ce que
	// timerMort == 1. A ce moment-l�, on le d�cr�mente encore -> il vaut 0 et on r�initialise
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

			//Sinon on r�initialise le niveau
			map->changeLevel(draw, joueur, init);
			initializePlayer(1, plateforme, map);

		}
	}
}



void Joueur::drawPlayer(map *map, Init1 *init)
{
	/* Gestion du timer */
	// Si notre timer (un compte � rebours en fait) arrive � z�ro
	if (player.frameTimer <= 0)
	{
		//On le r�initialise
		player.frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//Et on incr�mente notre variable qui compte les frames de 1 pour passer � la suivante
		player.frameNumber++;

		//Mais si on d�passe la frame max, il faut revenir � la premi�re :
		if (player.frameNumber >= player.frameMax)
			player.frameNumber = 0;

	}
	//Sinon, on d�cr�mente notre timer
	else
		player.frameTimer--;


	//Ensuite, on peut passer la main � notre fonction

	/* Rectangle de destination � dessiner */
	SDL_Rect dest;

	// On soustrait des coordonn�es de notre h�ros, ceux du d�but de la map, pour qu'il colle
	//au scrolling :
	dest.x = player.x - map->getStartX();
	dest.y = player.y - map->getStartY();
	dest.w = player.w;
	dest.h = player.h;

	/* Rectangle source */
	SDL_Rect src;

	//Pour conna�tre le X de la bonne frame � dessiner, il suffit de multiplier
	//la largeur du sprite par le num�ro de la frame � afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = player.frameNumber * player.w;
	src.w = player.w;
	src.h = player.h;

	//On calcule le Y de la bonne frame � dessiner, selon la valeur de l'�tat du h�ros :
	//Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
	//Tout cela en accord avec notre spritesheet, of course ;)
	src.y = player.etat * player.h;

	//Si on a �t� touch� et qu'on est invincible
	if (player.invincibleTimer > 0)
	{
		//On fait clignoter le h�ros une frame sur deux
		//Pour �a, on calcule si le num�ro de la frame en 
		//cours est un multiple de deux
		if (player.frameNumber % 2 == 0)
		{
			//Gestion du flip (retournement de l'image selon que le sprite regarde � droite ou � gauche
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
		//Gestion du flip (retournement de l'image selon que le sprite regarde � droite ou � gauche
		if (player.direction == LEFT)
			SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(init->getrenderer(), playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
	}

}




void Joueur::centerScrollingOnPlayer(map *map)
{
	// Nouveau scrolling � sous-bo�te limite :
	//Pour �viter les effets de saccades dus � une cam�ra qui se 
	//centre automatiquement et constamment sur le joueur (ce qui 
	//peut en rendre malade certains...), on cr�e une "bo�te" imaginaire 
	//autour du joueur. Quand on d�passe un de ses bords (en haut, en bas,
	//� gauche ou � droite), on scrolle.
	//Mais l� encore, au lieu de centrer sur le joueur, on d�place simplement
	//la cam�ra jusqu'� arriver au joueur. On a chang� ici la valeur � 4 pixels 
	//pour que le jeu soit plus rapide. 
	int cxperso = player.x + player.w / 2;
	int cyperso = player.y + player.h / 2;
	int xlimmin = map->getStartX() + LIMITE_X;
	int xlimmax = xlimmin + LIMITE_W;
	int ylimmin = map->getStartY() + LIMITE_Y;
	int ylimmax = ylimmin + LIMITE_H;

	//Effet de retour en arri�re quand on est mort :
	//Si on est tr�s loin de la cam�ra, plus loin que le bord
	//de la map, on acc�l�re le scrolling :
	if (cxperso < map->getStartX())
	{
		map->setStartX(map->getStartX() - 30);
	}

	//Si on d�passe par la gauche, on recule la cam�ra
	else if (cxperso < xlimmin)
	{
		map->setStartX(map->getStartX() - 4);
	}

	//Effet de retour en avant quand on est mort (au
	//cas o� le joueur s'amuse � faire le niveau � rebours
	//apr�s une checkpoint) :
	//Si on est tr�s loin de la cam�ra, plus loin que le bord
	//de la map, on acc�l�re le scrolling :
	if (cxperso > map->getStartX() + SCREEN_WIDTH)
	{
		map->setStartX(map->getStartX() + 30);
	}

	//Si on d�passe par la droite, on avance la cam�ra
	else if (cxperso > xlimmax)
	{
		map->setStartX(map->getStartX() + 4);
	}

	//Si on arrive au bout de la map � gauche, on stoppe le scrolling
	if (map->getStartX() < 0)
	{
		map->setStartX(0);
	}

	//Si on arrive au bout de la map � droite, on stoppe le scrolling � la 
	//valeur Max de la map - la moiti� d'un �cran (pour ne pas afficher du noir).
	else if (map->getStartX() + SCREEN_WIDTH >= map->getMaxX())
	{
		map->setStartX(map->getMaxX() - SCREEN_WIDTH);
	}

	//Si on d�passe par le haut, on remonte la cam�ra 
	if (cyperso < ylimmin)
	{
		map->setStartY(map->getStartY() - 4);
	}

	//Si on d�passe par le bas, on descend la cam�ra
	if (cyperso > ylimmax)
	{
		//Sauf si on tombe tr�s vite, auquel cas, on acc�l�re la cam�ra :
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

	//Si on arrive au bout de la map en bas, on stoppe le scrolling � la 
	//valeur Max de la map - la moiti� d'un �cran (pour ne pas afficher du noir ;) ).
	else if (map->getStartY() + SCREEN_HEIGHT >= map->getMaxY())
	{
		map->setStartY(map->getMaxY() - SCREEN_HEIGHT);
	}

}



void Joueur::getItem(int itemNumber, Son *son)
{
	switch (itemNumber)
	{
		//Gestion des �toiles
	case 1:
		//On incr�mente le compteur Etoile
		setNombreDetoiles(getNombreDetoiles() + 1);
		son->playSoundFx(3);

		//On teste s'il y a 100 �toiles : on remet le compteur � 0 et on rajoute une vie ;)
		if (getNombreDetoiles() >= 100)
		{
			setNombreDetoiles(0);
			//On incr�mente le nombre de vies (max 99)
			if (getNombreDeVies() < 99)
				setNombreDeVies(getNombreDeVies() + 1);
		}
		break;

		//Gestion des coeurs
	case 2:
		//On incr�mente le compteur Etoile
		if (player.life < 3)
			player.life++;

		son->playSoundFx(3);
		break;

		//Gestion des vies
	case 3:
		//On incr�mente le nombre de vies (max 99)
		if (getNombreDeVies() < 99)
			setNombreDeVies(getNombreDeVies() + 1);

		son->playSoundFx(3);
		break;

	default:
		break;
	}


}