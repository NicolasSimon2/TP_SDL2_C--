#include "input.h"
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


Input::Input()
{
}


Input::~Input()
{
}






void Input::openJoystick(void)
{
	//On ouvre le joystick
	joystick = SDL_JoystickOpen(0);

	if (!joystick)
		fprintf(stderr, "Le joystick 0 n'a pas pu être ouvert !\n");
}


void Input::closeJoystick(void)
{
	/* Ferme la prise en charge du joystick */
	if (SDL_JoystickGetAttached(joystick))
		SDL_JoystickClose(joystick);
}


void Input::gestionInputs(Input *input)
{
	/* On prend en compte les inputs (clavier, joystick... */
	if (joystick != NULL)
	{
		//On vérifie si le joystick est toujours connecté
		if (SDL_NumJoysticks() > 0)
			getJoystick(input);
		//Sinon on retourne au clavier
		else
		{
			SDL_JoystickClose(joystick);
			joystick = NULL;
		}

	}
	//S'il n'y a pas de manette on gère le clavier
	else
	{
		//On vérifie d'abord si une nouvelle manette a été branchée
		if (SDL_NumJoysticks() > 0)
		{
			//Si c'est le cas, on ouvre le joystick, qui sera opérationnel au prochain tour de boucle
			joystick = SDL_JoystickOpen(0);
			if (!joystick)
				fprintf(stderr, "Couldn't open Joystick 0\n");
		}
		//On gère le clavier
		getInput(input);
	}
}


void Input::getInput(Input *input)
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			case SDLK_DELETE:
				Input1.erase = 1;
				break;

			case SDLK_c:
				Input1.jump = 1;
				break;

			case SDLK_v:
				Input1.attack = 1;
				break;

			case SDLK_LEFT:
				Input1.left = 1;
				break;

			case SDLK_RIGHT:
				Input1.right = 1;
				break;

			case SDLK_DOWN:
				Input1.down = 1;
				break;

			case SDLK_UP:
				Input1.up = 1;
				break;


			case SDLK_RETURN:
				Input1.enter = 1;
				break;


			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_DELETE:
				Input1.erase = 0;
				break;

			case SDLK_c:
				Input1.jump = 0;
				break;

			case SDLK_LEFT:
				Input1.left = 0;
				break;

			case SDLK_RIGHT:
				Input1.right = 0;
				break;

			case SDLK_DOWN:
				Input1.down = 0;
				break;

			case SDLK_UP:
				Input1.up = 0;
				break;

			case SDLK_RETURN:
				Input1.enter = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getDown()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_DOWN:
				return 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_DOWN:
				return 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getUp()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_UP:
				Input1.up = 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_UP:
				Input1.up = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getRight()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_RIGHT:
				Input1.right = 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_RIGHT:
				Input1.right = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getLeft()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_LEFT:
				Input1.left = 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_LEFT:
				Input1.left = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getDelete()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_DELETE:
				Input1.erase = 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_DELETE:
				Input1.erase = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getC()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_c:
				Input1.jump = 1;
				break;

			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_c:
				Input1.jump = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

int Input::getEnter()
{
	SDL_Event event;
	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{

			case SDLK_RETURN:
				Input1.enter = 1;
				break;


			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{

			case SDLK_RETURN:
				Input1.enter = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

void Input::setDownZ()
{
	Input1.down = 0;
}

void Input::setUpZ()
{
	Input1.up = 0;
}

void Input::setRightZ()
{
	Input1.right = 0;
}

void Input::setLeftZ()
{
	Input1.left = 0;
}

void Input::setDeleteZ()
{
	Input1.erase = 0;
}

void Input::setEnterZ()
{
	Input1.enter = 0;
}

void Input::setCZ()
{
	Input1.jump = 0;
}




void Input::getJoystick(Input *input)
{
	SDL_Event event;
	

	//Si on ne touche pas au D-PAD, on le désactive (on teste les 4 boutons du D-PAD)
	if (SDL_JoystickGetButton(joystick, BOUTON_HAUT) == 0 && SDL_JoystickGetButton(joystick, BOUTON_BAS) == 0
		&& SDL_JoystickGetButton(joystick, BOUTON_DROITE) == 0 && SDL_JoystickGetButton(joystick, BOUTON_GAUCHE) == 0)
		DPADinUse = 0;

	/* On passe les events en revue */
	while (SDL_PollEvent(&event))
	{


		if (event.type == SDL_QUIT)
			exit(0);


		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			default:
				break;

			}
		}

		else if (event.type == SDL_JOYBUTTONDOWN)
		{

			if (event.jbutton.button == BOUTON_SAUT)
				Input1.jump = 1;

			else if (event.jbutton.button == BOUTON_ATTAQUE)
				Input1.attack = 1;

			else if (event.jbutton.button == BOUTON_PAUSE)
				Input1.enter = 1;

			else if (event.jbutton.button == BOUTON_QUIT)
				exit(0);

			else if (event.jbutton.button == BOUTON_HAUT)
			{
				Input1.up = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_BAS)
			{
				Input1.down = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_GAUCHE)
			{
				Input1.left = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_DROITE)
			{
				Input1.right = 1;
				DPADinUse = 1;
			}

		}

		else if (event.type == SDL_JOYBUTTONUP)
		{
			if (event.jbutton.button == BOUTON_PAUSE)
				Input1.enter = 0;

			else if (event.jbutton.button == BOUTON_SAUT)
				Input1.jump = 0;

			else if (event.jbutton.button == BOUTON_HAUT)
				Input1.up = 0;

			else if (event.jbutton.button == BOUTON_BAS)
				Input1.down = 0;

			else if (event.jbutton.button == BOUTON_GAUCHE)
				Input1.left = 0;

			else if (event.jbutton.button == BOUTON_DROITE)
				Input1.right = 0;

		}

		//Gestion du thumbpad, seulement si on n'utilise pas déjà le D-PAD
		else if (event.type == SDL_JOYAXISMOTION && DPADinUse == 0)
		{
			//Si le joystick a détecté un mouvement
			if (event.jaxis.which == 0)
			{
				//Si le mouvement a eu lieu sur l'axe des X
				if (event.jaxis.axis == 0)
				{
					//Si l'axe des X est neutre ou à l'intérieur de la "dead zone"
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						Input1.right = 0;
						Input1.left = 0;
					}
					//Sinon, de quel côté va-t-on ?
					else
					{
						//Si sa valeur est négative, on va à gauche
						if (event.jaxis.value < -DEAD_ZONE)
						{
							Input1.right = 0;
							Input1.left = 1;
						}
						//Sinon, on va à droite
						else if (event.jaxis.value > DEAD_ZONE)
						{
							Input1.right = 1;
							Input1.left = 0;
						}
					}
				}

				//Si le mouvement a eu lieu sur l'axe des Y
				else if (event.jaxis.axis == 1)
				{
					//Si l'axe des Y est neutre ou à l'intérieur de la "dead zone"
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						Input1.up = 0;
						Input1.down = 0;
					}
					//Sinon, de quel côté va-t-on ?
					else
					{
						//Si sa valeur est négative, on va en haut
						if (event.jaxis.value < 0)
						{
							Input1.up = 1;
							Input1.down = 0;
						}
						//Sinon, on va en bas
						else
						{
							Input1.up = 0;
							Input1.down = 1;
						}
					}
				}
			}
		}

	}

}