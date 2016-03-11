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
#include "SDL2/SDL_ttf.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

// Declare Variables - Shit Needs Cleaned, Mang
int quit;
int screen = 0;
int leftScore;
int rightScore;
char ScoreBuffer[10];
int xBallDirection = 1;
int yBallDirection = -1;
const Uint8 *currentKeyState;

// Declare SDL Pointers
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *ScoreSurface;
SDL_Texture *ScoreTexture;
SDL_Event e;

// Declare Rects
SDL_Rect startMessageRect;
SDL_Rect divider;
SDL_Rect ball;
SDL_Rect leftPaddle;
SDL_Rect rightPaddle;
SDL_Rect ScoreRect;

void defineRects()
{
    // Start Message
    startMessageRect.w = 240;
    startMessageRect.h = 24;
    startMessageRect.x = SCREEN_WIDTH - startMessageRect.w - 5;
    startMessageRect.y = SCREEN_HEIGHT - startMessageRect.h;
    //Divider
    startMessageRect.w = 480;
    startMessageRect.h = 64;
    startMessageRect.x = SCREEN_WIDTH / 2 + startMessageRect.w / 24;
    startMessageRect.y = SCREEN_HEIGHT / 4 - startMessageRect.h / 2;
    // Divider
    divider.w = 1;
    divider.h = SCREEN_HEIGHT;
    divider.x = SCREEN_WIDTH / 2;
    divider.y = 0;
    // Ball
    ball.w = 10;
    ball.h = 10;
    ball.x = SCREEN_WIDTH / 2 - ball.w / 2;
    ball.y = SCREEN_HEIGHT / 2 - ball.h / 2;
    // Left Paddle
    leftPaddle.w = 10;
    leftPaddle.h = 100;
    leftPaddle.x = 5;
    leftPaddle.y = SCREEN_HEIGHT / 2 - leftPaddle.h / 2;
    // Right Paddle
    rightPaddle.w = 10;
    rightPaddle.h = 100;
    rightPaddle.x = SCREEN_WIDTH - rightPaddle.w - 5;
    rightPaddle.y = SCREEN_HEIGHT / 2 - rightPaddle.h / 2;
    // Left Score
    ScoreRect.w = 0; // All three of these are now zeroed as they're computed later.
    ScoreRect.h = 0;
    ScoreRect.x = 0;
    ScoreRect.y = 32;
}

void getKeystates()
{
    currentKeyState = SDL_GetKeyboardState(NULL);
    if(currentKeyState[SDL_SCANCODE_W]) leftPaddle.y--;
    if(currentKeyState[SDL_SCANCODE_S]) leftPaddle.y++;
    if(currentKeyState[SDL_SCANCODE_UP]) rightPaddle.y--;
    if(currentKeyState[SDL_SCANCODE_DOWN]) rightPaddle.y++;
    if(currentKeyState[SDL_SCANCODE_RETURN]) screen = 1;
    if(currentKeyState[SDL_SCANCODE_ESCAPE]) screen = 0;
}

void paddleMovement()
{
    if(leftPaddle.y < 0) leftPaddle.y++;
    if(leftPaddle.y + leftPaddle.h > SCREEN_HEIGHT) leftPaddle.y--;
    if(rightPaddle.y < 0) rightPaddle.y++;
    if(rightPaddle.y + leftPaddle.h > SCREEN_HEIGHT) rightPaddle.y--;
}

void ballPhysics()
{
    // Velocity
    ball.x = ball.x + xBallDirection;
    ball.y = ball.y + yBallDirection;
    // Balls to the Walls
    if(ball.x < 0) rightScore++, ball.x = SCREEN_WIDTH / 2 - ball.w / 2, xBallDirection = 1;
    if(ball.x + ball.w > SCREEN_WIDTH) leftScore++, ball.x = SCREEN_WIDTH / 2 - ball.w / 2, xBallDirection = -1;
    if(ball.y < 0) yBallDirection = 1;
    if(ball.y + ball.h > SCREEN_HEIGHT) yBallDirection = -1;
    // Paddle to the Balls
    if(ball.x < leftPaddle.x + leftPaddle.w && ball.x + ball.w > leftPaddle.x &&
       ball.y < leftPaddle.y + leftPaddle.h && ball.h + ball.y > leftPaddle.y)
    {
        xBallDirection = 1;
    }
    if(ball.x < rightPaddle.x + rightPaddle.w && ball.x + ball.w > rightPaddle.x &&
       ball.y < rightPaddle.y + rightPaddle.h && ball.h + ball.y > rightPaddle.y)
    {
        xBallDirection = -1;
    }
}

void clearScreen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void renderGame()
{
    // Divider
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &divider);
    SDL_RenderFillRect(renderer, &divider);
    // Left Paddle
    SDL_RenderDrawRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &leftPaddle);
    // Right Paddle
    SDL_RenderDrawRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &rightPaddle);
    // Ball
    SDL_RenderDrawRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &ball);
    // Scoreboard
    SDL_RenderCopy(renderer, ScoreTexture, NULL, &ScoreRect);
    // Draw
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("Simple Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("fonts/slkscr.ttf", 16);
    SDL_Color white = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Press Enter to Start", white);
    SDL_Texture *startMessageTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    defineRects();

    // Main Game Loop
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = 1;
        }

		getKeystates();
        paddleMovement();

		// Scoreboard
        sprintf(ScoreBuffer, "%d   %d", rightScore, leftScore);
		ScoreSurface = TTF_RenderText_Solid(font, ScoreBuffer, white); // Get score to surface.
        ScoreTexture = SDL_CreateTextureFromSurface(renderer, ScoreSurface); // Make it a texture.
        SDL_FreeSurface(ScoreSurface); // Don't need the surface anymore since it's now a texture.
        SDL_QueryTexture(ScoreTexture,NULL,NULL,&ScoreRect.w,&ScoreRect.h); // ScoreRect now contains the width and height of our scoreboard.
        ScoreRect.w*=4; // Adjust font size manually here from texture size queried.
        ScoreRect.h*=4; // Adjust font size manually here from texture size queried.
        ScoreRect.x = SCREEN_WIDTH / 2  - ScoreRect.w / 2; // Put it at center, minus half current width.

		switch(screen)
		{
            case 0:
                clearScreen();
                SDL_RenderCopy(renderer, startMessageTexture, NULL, &startMessageRect);
                renderGame();
                break;
            case 1:
                ballPhysics();
                clearScreen();
                renderGame();
                break;
		}

        SDL_Delay(2);
    }

    // Quit SDL
    TTF_CloseFont(font);
    SDL_DestroyTexture(ScoreTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
