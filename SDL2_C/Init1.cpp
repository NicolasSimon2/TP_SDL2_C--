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



Init1::Init1()
{
}


Init1::~Init1()
{
}


SDL_Renderer *Init1::getrenderer(void)
{
	return renderer;
}


void Init1::init(char *title, Font *font, Input *input)
{
	/* On crée la fenêtre, représentée par le pointeur jeu.window en utilisant la largeur et la
	hauteur définies dans les defines (defs.h).
	Nouveautés SDL2 : on peut centrer la fenêtre avec SDL_WINDOWPOS_CENTERED, et choisir la taille
	de la fenêtre, pour que la carte graphique l'agrandisse automatiquement. Notez aussi qu'on peut
	maintenant créer plusieurs fenêtres. */



	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {

		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		//return 1;
	}

	screen = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	//On crée un renderer pour la SDL et on active la synchro verticale : VSYNC
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);

	// Si on n'y arrive pas, on quitte en enregistrant l'erreur dans stdout.txt
	if (screen == NULL || renderer == NULL)
	{
		fprintf(stderr, "Impossible d'initialiser le mode écran à %d x %d: %s\n", SCREEN_WIDTH,
			SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	//Initialisation du chargement des images png avec SDL_Image 2
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError());
		exit(1);
	}

	//On cache le curseur de la souris 
	SDL_ShowCursor(SDL_DISABLE);

	//On initialise SDL_TTF 2 qui gérera l'écriture de texte
	if (TTF_Init() < 0)
	{
		fprintf(stderr, "Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
		exit(1);
	}

	/* Chargement de la police */
	font->loadFont("font/GenBasB.ttf", 32);

	//On initialise SDL_Mixer 2, qui gérera la musique et les effets sonores
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags)
	{
		fprintf(stderr, "Mix_Init: Failed to init SDL_Mixer\n");
		fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
		exit(1);
	}

	/* Open 44.1KHz, signed 16bit, system byte order,
	stereo audio, using 1024 byte chunks (voir la doc pour plus d'infos ;) ) */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		exit(1);
	}

	// Définit le nombre de pistes audio (channels) à mixer
	Mix_AllocateChannels(32);

	// Initialise le sous-sytème de la SDL gérant les joysticks
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	//On cherche s'il y a des joysticks
	if (SDL_NumJoysticks() > 0)
	{
		input->openJoystick();
	}

}

void Init1::loadGame(map *map, menu *menu, Joueur *joueur, Draw *draw, plateforme *plateforme, Son *son, Init1 *init)
{

	//On charge les données pour la map
	map->initMaps(draw, init);

	//On initialise les menus
	menu->initMenus(init, draw);



	//On charge la feuille de sprites (spritesheet) de notre héros 
	joueur->initPlayerSprites(draw, init);

	//On commence au premier niveau
	joueur->SetValeurDuNiveau(1);
	map->changeLevel(draw, joueur, init);

	/* On initialise les variables du jeu */
	joueur->setNombreDeVies(3);
	joueur->setNombreDetoiles(0);

	/* On charge le HUD */
	draw->initHUD(init);


	//On charge notre plateforme
	plateforme->loadPlateforme(draw, init);

	//On charge la musique
	son->loadSong("music/Caviator.mp3");

	/* On charge les sounds Fx */
	son->loadSound();

	//On commence par le menu start
	menu->setOnMenu(1, 1/*start*/);

}


void Init1::cleanup(map *map, menu *menu, Joueur *joueur, Draw *draw, plateforme *plateforme, Son *son, Input *input)
{

	//Nettoie les sprites de la map et des menus
	map->cleanMaps();
	menu->cleanMenus();

	/* Libère le sprite du héros */
	joueur->cleanPlayer();



	//Libère le HUD
	draw->cleanHUD();

	//Libère la plateforme
	plateforme->cleanPlateforme();

	/* Ferme la prise en charge du joystick */
	input->closeJoystick();

	/* On libère la musique */
	son->cleanUpMusic();

	//On libère les sons
	son->freeSound();

	//On quitte SDL_Mixer 2 et on décharge la mémoire
	Mix_CloseAudio();
	Mix_Quit();

	//On fait le ménage et on remet les pointeurs à NULL
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(screen);
	screen = NULL;

	//On quitte SDL_TTF 2
	TTF_Quit();

	//On quitte la SDL 
	SDL_Quit();
}




