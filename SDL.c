#include "main.h"
#include "SDL.h"

SDL_Color white = {255, 255, 255, 255};

void initializeSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("Simple Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    silkscreen = TTF_OpenFont("fonts/slkscr.ttf", 16); // Open Font
    textSurface = TTF_RenderText_Solid(silkscreen, "Press Enter to Start", white);
    startMessageTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void quitSDL()
{
    TTF_CloseFont(silkscreen);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(startButtonTexture);
    SDL_FreeSurface(startButtonSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
