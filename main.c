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
int xBallDirection = 1;
int yBallDirection = -1;
const Uint8 *currentKeyState;

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
        // Left Paddle
    SDL_Rect leftPaddle;
    leftPaddle.w = 10;
    leftPaddle.h = 100;
    leftPaddle.x = 0;
    leftPaddle.y = SCREEN_HEIGHT / 2 - leftPaddle.h / 2;
        // Right Paddle
    SDL_Rect rightPaddle;
    rightPaddle.w = 10;
    rightPaddle.h = 100;
    rightPaddle.x = SCREEN_WIDTH - rightPaddle.w;
    rightPaddle.y = SCREEN_HEIGHT / 2 - rightPaddle.h / 2;
        // Ball
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
        }

        ball.x = ball.x + xBallDirection;
        ball.y = ball.y + yBallDirection;

		// Get Keystates
		currentKeyState = SDL_GetKeyboardState(NULL);
		if(currentKeyState[SDL_SCANCODE_W]) leftPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_S]) leftPaddle.y++;
		if(currentKeyState[SDL_SCANCODE_UP]) rightPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_DOWN]) rightPaddle.y++;

		// Logic
		if(ball.x < 0) xBallDirection = 1;
		if(ball.x + ball.w > SCREEN_WIDTH) xBallDirection = -1;
		if(ball.y < 0) yBallDirection = 1;
		if(ball.y + ball.h > SCREEN_HEIGHT) yBallDirection = -1;
		if(leftPaddle.y < 0) leftPaddle.y++;
		if(leftPaddle.y + leftPaddle.h > SCREEN_HEIGHT) leftPaddle.y--;
		if(rightPaddle.y < 0) rightPaddle.y++;
		if(rightPaddle.y + leftPaddle.h > SCREEN_HEIGHT) rightPaddle.y--;

		// Render
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
            // Left Paddle
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &leftPaddle);
            // Right Paddle
        SDL_RenderDrawRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
            // Ball
        SDL_RenderDrawRect(renderer, &ball);
        SDL_RenderFillRect(renderer, &ball);
            // Update Screen
        SDL_RenderPresent(renderer);
    }

    // Quit SDL
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
