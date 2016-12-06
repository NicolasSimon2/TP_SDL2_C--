//Rabidja 3 - nouvelle version int�gralement en SDL 2.0 
//Copyright / Droits d'auteur : www.meruvia.fr - J�r�mie F. Bellanger


#include "prototypes.h"



int nombreMonstres;
GameObject monster[MONSTRES_MAX];
SDL_Texture *MonsterSprite;


GameObject *getMonster(int nombre)
{
	return &monster[nombre];
}

int getMonsterNumber(void)
{
	return nombreMonstres;
}

void resetMonsters(void)
{
	nombreMonstres = 0;
}

void initMonsterSprites(void)
{
	MonsterSprite = loadImage("graphics/monster1.png");
}

void cleanMonsters(void)
{
	/* Lib�re le sprite des monstres */
	if (MonsterSprite != NULL)
	{
		SDL_DestroyTexture(MonsterSprite);
		MonsterSprite = NULL;
	}
}

void initializeNewMonster(int x, int y)
{
	//Si on n'est pas rendu au max, on rajoute un monstre dont le num�ro est �gal
	//� nombreMonstres : monster[0] si c'est le 1er, monster[1], si c'est le 2eme, etc...

	if (nombreMonstres < MONSTRES_MAX)
	{

		//On r�initialise la frame et le timer
		monster[nombreMonstres].frameNumber = 0;
		monster[nombreMonstres].frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//On indique sa direction (il viendra � l'inverse du joueur, logique)
		monster[nombreMonstres].direction = LEFT;

		/* Ses coordonn�es de d�marrage seront envoy�es par la fonction drawMap() en arguments */
		monster[nombreMonstres].x = x;
		monster[nombreMonstres].y = y;

		/* Hauteur et largeur de notre monstre (rajout� dans les defs ;) ) */
		monster[nombreMonstres].w = MONSTER_WIDTH;
		monster[nombreMonstres].h = MONSTER_HEIGTH;


		//Variables n�cessaires au fonctionnement de la gestion des collisions comme pour le h�ros
		monster[nombreMonstres].timerMort = 0;
		monster[nombreMonstres].onGround = 0;

		nombreMonstres++;

	}


}


void updateMonsters()
{
	int i;

	//On passe en boucle tous les monstres du tableau
	for (i = 0; i < nombreMonstres; i++)
	{
		//M�me fonctionnement que pour le joueur
		if (monster[i].timerMort == 0)
		{

			monster[i].dirX = 0;
			monster[i].dirY += GRAVITY_SPEED;

			if (monster[i].dirY >= MAX_FALL_SPEED)
				monster[i].dirY = MAX_FALL_SPEED;

			//Test de collision dans un mur : si la variable x reste la m�me, deux tours de boucle
			//durant, le monstre est bloqu� et on lui fait faire demi-tour.
			if (monster[i].x == monster[i].saveX || checkFall(monster[i]) == 1)
			{
				if (monster[i].direction == LEFT)
				{
					monster[i].direction = RIGHT;
				}
				else
				{
					monster[i].direction = LEFT;
				}

			}

			//D�placement du monstre selon la direction
			if (monster[i].direction == LEFT)
				monster[i].dirX -= 2;
			else
				monster[i].dirX += 2;


			//On sauvegarde les coordonn�es du monstre pour g�rer le demi-tour
			//avant que mapCollision ne les modifie.
			monster[i].saveX = monster[i].x;

			//On d�tecte les collisions avec la map comme pour le joueur
			monsterCollisionToMap(&monster[i]);

			// Test de collision monstre-boule de feu
			if (getShurikenNumber() > 0)
			{
				if (shurikenCollide(&monster[i]))
				{
					//On met le timer � 1 pour tuer le monstre intantan�ment
					monster[i].timerMort = 1;
					playSoundFx(DESTROY);
				}
			}

			//On d�tecte les collisions avec le joueur
			//Si c'est �gal � 1, on diminue ses PV
			if (collide(getPlayer(), &monster[i]) == 1)
			{
				if (getLife() > 1)
				{
					playerHurts(&monster[i]);
				}
				else
				{
					killPlayer();
				}

			}
			else if (collide(getPlayer(), &monster[i]) == 2)
			{
				//On met le timer � 1 pour tuer le monstre intantan�ment
				monster[i].timerMort = 1;
				playSoundFx(DESTROY);
			}

		}

		//Si le monstre meurt, on active une tempo
		if (monster[i].timerMort > 0)
		{
			monster[i].timerMort--;

			/* Et on le remplace simplement par le dernier du tableau puis on
			r�tr�cit le tableau d'une case (on ne peut pas laisser de case vide) */
			if (monster[i].timerMort == 0)
			{

				monster[i] = monster[nombreMonstres - 1];
				nombreMonstres--;

			}
		}
	}

}


//Fonction de gestion des collisions
int collide(GameObject *player, GameObject *monster)
{
	//On teste pour voir s'il n'y a pas collision, si c'est le cas, on renvoie 0
	if ((player->x >= monster->x + monster->w)
		|| (player->x + player->w <= monster->x)
		|| (player->y >= monster->y + monster->h)
		|| (player->y + player->h <= monster->y)
		)
		return 0;
	//Sinon, il y a collision. Si le joueur est au-dessus du monstre (avec une marge
	//de 10 pixels pour �viter les frustrations dues au pixel perfect), on renvoie 2.
	//On devra alors tuer le monstre et on fera rebondir le joueur.
	else if (player->y + player->h <= monster->y + 10)
	{
		player->dirY = -JUMP_HEIGHT;
		return 2;
	}
	//Sinon, on renvoie 1 et c'est le joueur qui meurt...
	else
		return 1;
}



int checkFall(GameObject monster)
{
	int x, y;

	//Fonction qui teste s'il y a du sol sous un monstre
	//Retourne 1 s'il doit tomber, 0 sinon

	//On teste la direction, pour savoir si on doit prendre en compte x ou x + w (cf. sch�ma)
	if (monster.direction == LEFT)
	{
		//On va � gauche : on calcule l� o� devrait se trouver le monstre apr�s d�placement.
		//S'il sort de la map, on met � jour x et y pour �viter de sortir de notre tableau
		//(source d'erreur possible qui peut planter notre jeu...).
		x = (int)(monster.x + monster.dirX) / TILE_SIZE;
		y = (int)(monster.y + monster.h - 1) / TILE_SIZE;
		if (y < 0)
			y = 1;
		if (y > MAX_MAP_Y)
			y = MAX_MAP_Y;
		if (x < 0)
			x = 1;
		if (x > MAX_MAP_X)
			x = MAX_MAP_X;

		//On teste si la tile sous le monstre est traversable (du vide quoi...).
		//Si c'est le cas, on renvoie 1, sinon 0.
		if (getTileValue(y + 1, x) < BLANK_TILE - 20)
			return 1;

		else
			return 0;
	}
	else
	{
		//M�me chose quand on va � droite
		x = (int)(monster.x + monster.w + monster.dirX) / TILE_SIZE;
		y = (int)(monster.y + monster.h - 1) / TILE_SIZE;
		if (y <= 0)
			y = 1;
		if (y >= MAX_MAP_Y)
			y = MAX_MAP_Y - 1;
		if (x <= 0)
			x = 1;
		if (x >= MAX_MAP_X)
			x = MAX_MAP_X - 1;

		if (getTileValue(y + 1, x) < BLANK_TILE - 20)
			return 1;

		else
			return 0;
	}
}


void drawMonster(GameObject *entity)
{
	/* Gestion du timer */
	// Si notre timer (un compte � rebours en fait) arrive � z�ro
	if (entity->frameTimer <= 0)
	{
		//On le r�initialise
		entity->frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//Et on incr�mente notre variable qui compte les frames de 1 pour passer � la suivante
		entity->frameNumber++;

		//Mais si on d�passe la frame max, il faut revenir � la premi�re, sauf si c'est une animation unique
		//(qui ne s'ex�cute qu'une seule fois comme les explosions), auquel cas, OneTimeAnimation vaudra 1.
		//SDL2 : on retrouve la largeur de la feuille de sprite
		int w;
		SDL_QueryTexture(MonsterSprite, NULL, NULL, &w, NULL);
		if (entity->frameNumber >= w / entity->w)
			entity->frameNumber = 0;

	}
	//Sinon, on d�cr�mente notre timer
	else
		entity->frameTimer--;


	//Ensuite, on peut passer la main � notre fonction

	/* Rectangle de destination � dessiner */
	SDL_Rect dest;

	// On soustrait des coordonn�es de notre monstre, ceux du d�but de la map, pour qu'il colle
	//au scrolling :
	dest.x = entity->x - getStartX();
	dest.y = entity->y - getStartY();
	dest.w = entity->w;
	dest.h = entity->h;

	/* Rectangle source */
	SDL_Rect src;

	//Pour conna�tre le X de la bonne frame � dessiner, il suffit de multiplier
	//la largeur du sprite par le num�ro de la frame � afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = entity->frameNumber * entity->w;
	src.y = 0;
	src.w = entity->w;
	src.h = entity->h;

	//Gestion du flip (retournement de l'image selon que le monstre regarde � droite ou � gauche
	if (entity->direction == LEFT)
		SDL_RenderCopyEx(getrenderer(), MonsterSprite, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(getrenderer(), MonsterSprite, &src, &dest, 0, 0, SDL_FLIP_NONE);


}





