#ifndef SDL_H_INCLUDED
#define SDL_H_INCLUDED

#include "main.h"

int quit;
int screen;

// Declare SDL Assets
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *silkscreen;
SDL_Event e;
SDL_Rect ball;
SDL_Color white;

SDL_Surface *titleSurface;
SDL_Texture *titleTexture;

SDL_Surface *textSurface;
SDL_Texture *startMessageTexture;

SDL_Surface *startButtonSurface;
SDL_Texture *startButtonTexture;

void initializeSDL();
void quitSDL();

#endif // SDL_H_INCLUDED
