#include "font.h"
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


Font::Font()
{
}


Font::~Font()
{
}






void Font::loadFont(char *name, int size)
{
	/* Use SDL_TTF pour charger la police */

	font = TTF_OpenFont(name, size);

	if (font == NULL)
	{
		fprintf(stderr, "Failed to open Font %s: %s\n", name, TTF_GetError());
		exit(1);
	}

}


void Font::closeFont()
{
	/* Ferme police */

	if (font != NULL)
	{
		TTF_CloseFont(font);
	}
}


void Font::drawString(char *text, int x, int y, int r, int g, int b, int a, Init1 *init)
{
	SDL_Rect dest;
	SDL_Surface *surface; //Pour écrire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture
	SDL_Color foregroundColor;

	/* Couleur du texte RGBA */
	foregroundColor.r = r;
	foregroundColor.g = g;
	foregroundColor.b = b;
	foregroundColor.a = a;


	/* On utilise SDL_TTF pour générer une SDL_Surface à partir d'une chaîne de caractères (string) */
	surface = TTF_RenderUTF8_Blended(font, text, foregroundColor);

	if (surface != NULL)
	{
		/* NOUS MODIFIONS QUELQUE PEU NOTRE CODE POUR PROFITER DE LA MEMOIRE GRAPHIQUE
		QUI GERE LES TEXTURES  */
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(init->getrenderer(), surface);

		// On se débarrasse du pointeur vers une surface
		/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre
		dédié) */
		SDL_FreeSurface(surface);
		surface = NULL;

		// On dessine cette texture à l'écran 
		dest.x = x;
		dest.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(init->getrenderer(), texture, NULL, &dest);

		//On supprime la texture
		SDL_DestroyTexture(texture);

	}
	else
	{
		fprintf(stderr, "La chaine n'a pu être écrite %s: %s\n", text, TTF_GetError());

		exit(0);
	}
}
