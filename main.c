/*
Copyright (C) 2016  Marcus Law

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

int quit;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *surface;
SDL_Event e;

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Simple Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Declare Rects
    SDL_Rect leftPaddle;
    leftPaddle.w = 10;
    leftPaddle.h = 100;
    leftPaddle.x = 0;
    leftPaddle.y = SCREEN_HEIGHT / 2 - leftPaddle.h / 2;

    SDL_Rect rightPaddle;
    rightPaddle.w = 10;
    rightPaddle.h = 100;
    rightPaddle.x = SCREEN_WIDTH - rightPaddle.w;
    rightPaddle.y = SCREEN_HEIGHT / 2 - rightPaddle.h / 2;

    SDL_Rect ball;
    ball.w = 10;
    ball.h = 10;
    ball.x = SCREEN_WIDTH / 2 - ball.w / 2;
    ball.y = SCREEN_HEIGHT / 2 - ball.h / 2;

    // Main Game Loop
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = 1;
            else if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_w:
                        leftPaddle.y = leftPaddle.y - 10;
                        break;
                    case SDLK_s:
                        leftPaddle.y = leftPaddle.y + 10;
                        break;
                    case SDLK_UP:
                        rightPaddle.y = rightPaddle.y - 10;
                        break;
                    case SDLK_DOWN:
                        rightPaddle.y = rightPaddle.y + 10;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &leftPaddle);

        SDL_RenderDrawRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);

        SDL_RenderDrawRect(renderer, &ball);
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
    }

    // Quit SDL
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
