#include "menu.h"
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Init1.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"
#include "stdio.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"


menu::menu()
{
}


menu::~menu()
{
}

//Gestion des menus



int menu::getOnMenu(void)
{
	return onMenu;
}

int menu::getMenuType(void)
{
	return menuType;
}

void menu::setOnMenu(int valeur, int type)
{
	onMenu = valeur;
	menuType = type;
}

void menu::initMenus(Init1 *init, Draw *draw)
{
	
	titlescreen = draw->loadImage("graphics/title.png", init);
}

void menu::cleanMenus(void)
{
	// Libère la texture de l'écran-titre 
	if (titlescreen != NULL)
	{
		SDL_DestroyTexture(titlescreen);
		titlescreen = NULL;
	}

}


void menu::updateStartMenu(Input *input, Joueur *joueur, map *map, plateforme *plateforme, Draw *draw, Init1 *init)
{
	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDownZ();
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUpZ();
	}

	//Choix du level
	if (input->getRight() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (joueur->getLevel() >= LEVEL_MAX)
			joueur->SetValeurDuNiveau(1);
		else
			joueur->SetValeurDuNiveau(joueur->getLevel() + 1);

		input->setRightZ();
	}

	if (input->getLeft() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (joueur->getLevel() <= 1)
			joueur->SetValeurDuNiveau(LEVEL_MAX);
		else
			joueur->SetValeurDuNiveau(joueur->getLevel() - 1);

		input->setLeftZ();
	}



	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getC())
	{
		if (choice == 0)
		{
			joueur->resetCheckpoint();
			joueur->initializePlayer(1, plateforme, map);
			map->changeLevel(draw, joueur, init);
			/* On réinitialise les variables du jeu */
			joueur->setNombreDeVies(3);
			joueur->setNombreDetoiles(0);
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			exit(0);
		}
		input->setEnterZ();
		input->setCZ();
	}


}



void menu::updatePauseMenu(Input *input)
{

	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDownZ();
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUpZ();
	}

	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getC())
	{
		if (choice == 0)
		{
			//Si on appuie sur Enter on quitte l'état menu
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			choice = 0;
			menuType = START;
		}
		input->setEnterZ();
		input->setCZ();
	}

}


void menu::drawStartMenu(Draw *draw, Init1 *init, Font *font, Joueur *joueur)
{

	//On crée une variable qui contiendra notre texte
	char text[200];

	//On affiche l'écran-titre
	draw->drawImage(titlescreen, 0, 0, init);

	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", joueur->getLevel());
		//Ombrage en noir
		font->drawString(text, 375, 252, 0, 0, 0, 255, init);
		font->drawString(text, 373, 250, 255, 255, 255, 255, init);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		font->drawString(text, 425, 292, 0, 0, 0, 255, init);
		font->drawString(text, 422, 290, 255, 255, 255, 255, init);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", joueur->getLevel());
		//Ombrage en noir
		font->drawString(text, 375, 252, 0, 0, 0, 255, init);
		font->drawString(text, 373, 250, 255, 255, 0, 255, init);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		font->drawString(text, 425, 292, 0, 0, 0, 255, init);
		font->drawString(text, 422, 290, 255, 255, 0, 255, init);
	}


}



void menu::drawPauseMenu(Init1 *init, Font *font)
{
	char text[200];

	//On écrit PAUSE

	sprintf_s(text, sizeof(text), "** PAUSE **");
	font->drawString(text, 322, 200, 0, 0, 0, 255, init);
	font->drawString(text, 320, 198, 255, 255, 255, 255, init);


	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		font->drawString(text, 346, 252, 0, 0, 0, 255, init);
		font->drawString(text, 344, 250, 255, 255, 255, 255, init);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		font->drawString(text, 386, 292, 0, 0, 0, 255, init);
		font->drawString(text, 384, 290, 255, 255, 255, 255, init);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		font->drawString(text, 346, 252, 0, 0, 0, 255, init);
		font->drawString(text, 344, 250, 255, 255, 0, 255, init);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		font->drawString(text, 386, 292, 0, 0, 0, 255, init);
		font->drawString(text, 384, 290, 255, 255, 0, 255, init);
	}


}
