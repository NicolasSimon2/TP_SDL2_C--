#include "Init1.h"
#include "stdio.h"
#include "SDL_ttf.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "input.h"
#include "Draw.h"
#include "font.h"
#include "Son.h"
#include "Joueur.h"
#include "map.h"
#include "menu.h"
#include "Plateforme.h"






int main(int argc, char *argv[])
{

	Input input;
	Draw draw;
	Font font;
	Init1 init1;
	Joueur joueur;
	map map;
	menu menu;
	plateforme plateforme;
	Son son;

	unsigned int frameLimit = SDL_GetTicks() + 16;
	int go;


	// Initialisation de la SDL 
	init1.init("Rabidja 3 - SDL 2 - Simon Nicolas", &font, &input);


	// Chargement des ressources (graphismes, sons) 
	init1.loadGame(&map, &menu, &joueur, &draw, &plateforme, &son, &init1);

	/* On initialise le joueur */
	joueur.initializePlayer(1, &plateforme, &map);


	// Appelle la fonction cleanup à la fin du programme 
	//atexit(init1.cleanup(map,  menu, joueur, draw, plateforme, son, input));

	go = 1;

	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		getchar();
		return 1;
	}


	// Boucle infinie, principale, du jeu 
	while (go == 1)
	{

		//Gestion des inputs et du joystick
		input.gestionInputs(&input);

		//Si on n'est pas dans un menu
		if (menu.getOnMenu() == 0)
		{
			/* On met à jour le jeu */
			joueur.updatePlayer(&input, &son, &menu, &map, &joueur, &plateforme, &draw, &init1);

			plateforme.doPlateforme(&joueur);



		}
		else
		{
			if (menu.getMenuType() == 0)
				menu.updateStartMenu(&input, &joueur, &map, &plateforme, &draw, &init1);
			else if (menu.getMenuType() == 1)
				menu.updatePauseMenu(&input);
		}


		//Si on n'est pas dans un menu, on affiche le jeu
		if (menu.getOnMenu() == 0)
			draw.drawGame(0, &map, &joueur, &plateforme, &menu, &init1, &draw, &font);
		//continue;


		else
		{
			if (menu.getMenuType() == 0)
			{
				menu.drawStartMenu(&draw, &init1, &font, &joueur);
				SDL_RenderPresent(init1.getrenderer());
				SDL_Delay(1);
			}
			else if (menu.getMenuType() == 1)
				draw.drawGame(1, &map, &joueur, &plateforme, &menu, &init1, &draw, &font);

		}

		// Gestion des 60 fps (1000ms/60 = 16.6 . 16 
		draw.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	getchar();
	return 0;

}