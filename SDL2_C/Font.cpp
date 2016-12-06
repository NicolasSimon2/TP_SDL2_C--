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
	SDL_Surface *surface; //Pour �crire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture
	SDL_Color foregroundColor;

	/* Couleur du texte RGBA */
	foregroundColor.r = r;
	foregroundColor.g = g;
	foregroundColor.b = b;
	foregroundColor.a = a;


	/* On utilise SDL_TTF pour g�n�rer une SDL_Surface � partir d'une cha�ne de caract�res (string) */
	surface = TTF_RenderUTF8_Blended(font, text, foregroundColor);

	if (surface != NULL)
	{
		/* NOUS MODIFIONS QUELQUE PEU NOTRE CODE POUR PROFITER DE LA MEMOIRE GRAPHIQUE
		QUI GERE LES TEXTURES  */
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(init->getrenderer(), surface);

		// On se d�barrasse du pointeur vers une surface
		/* On lib�re la SDL_Surface temporaire (pour �viter les fuites de m�moire - cf. chapitre
		d�di�) */
		SDL_FreeSurface(surface);
		surface = NULL;

		// On dessine cette texture � l'�cran 
		dest.x = x;
		dest.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(init->getrenderer(), texture, NULL, &dest);

		//On supprime la texture
		SDL_DestroyTexture(texture);

	}
	else
	{
		fprintf(stderr, "La chaine n'a pu �tre �crite %s: %s\n", text, TTF_GetError());

		exit(0);
	}
}
