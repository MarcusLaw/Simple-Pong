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
#include "SDL2/SDL_ttf.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

int quit;
int screen = 0;
int leftScore;
int rightScore;
char leftScoreBuffer[10];
char rightScoreBuffer[10];
int xBallDirection = 1;
int yBallDirection = -1;
const Uint8 *currentKeyState;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *leftScoreSurface;
SDL_Surface *rightScoreSurface;
SDL_Texture *leftScoreTexture;
SDL_Texture *rightScoreTexture;
SDL_Event e;

// Declare Rects
SDL_Rect startMessageRect;
SDL_Rect divider;
SDL_Rect ball;
SDL_Rect leftPaddle;
SDL_Rect rightPaddle;
SDL_Rect leftScoreRect;
SDL_Rect rightScoreRect;

void defineRects()
{
    // Start Message
    startMessageRect.w = 480;
    startMessageRect.h = 64;
    startMessageRect.x = SCREEN_WIDTH / 2 + startMessageRect.w / 24;
    startMessageRect.y = SCREEN_HEIGHT / 4 - startMessageRect.h / 2;
    //Divider
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
    leftScoreRect.w = 64;
    leftScoreRect.h = 64;
    leftScoreRect.x = SCREEN_WIDTH / 2 - leftScoreRect.w / 2 - 50;
    leftScoreRect.y = 32;
    // Right Score
    rightScoreRect.w = 64;
    rightScoreRect.h = 64;
    rightScoreRect.x = SCREEN_WIDTH / 2 - rightScoreRect.w / 2 + 50;
    rightScoreRect.y = 32;
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
    SDL_RenderCopy(renderer, leftScoreTexture, NULL, &leftScoreRect);
    SDL_RenderCopy(renderer, rightScoreTexture, NULL, &rightScoreRect);
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
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Press Space to Start", white);
    SDL_Texture *startMessageTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    defineRects();

    // Main Game Loop
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = 1;
        }

		// Get Keystates
		currentKeyState = SDL_GetKeyboardState(NULL);
		if(currentKeyState[SDL_SCANCODE_W]) leftPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_S]) leftPaddle.y++;
		if(currentKeyState[SDL_SCANCODE_UP]) rightPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_DOWN]) rightPaddle.y++;
		if(currentKeyState[SDL_SCANCODE_SPACE]) screen = 1;
		if(currentKeyState[SDL_SCANCODE_ESCAPE]) screen = 0;

        // Paddles
		if(leftPaddle.y < 0) leftPaddle.y++;
		if(leftPaddle.y + leftPaddle.h > SCREEN_HEIGHT) leftPaddle.y--;
		if(rightPaddle.y < 0) rightPaddle.y++;
		if(rightPaddle.y + leftPaddle.h > SCREEN_HEIGHT) rightPaddle.y--;

		// Score Board
		sprintf(leftScoreBuffer, "%d", leftScore);
        sprintf(rightScoreBuffer, "%d", rightScore);
		leftScoreSurface = TTF_RenderText_Solid(font, leftScoreBuffer, white);
		rightScoreSurface = TTF_RenderText_Solid(font, rightScoreBuffer, white);
		leftScoreTexture = SDL_CreateTextureFromSurface(renderer, leftScoreSurface);
		rightScoreTexture = SDL_CreateTextureFromSurface(renderer, rightScoreSurface);

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

        SDL_Delay(3);
    }

    // Quit SDL
    TTF_CloseFont(font);
    SDL_DestroyTexture(leftScoreTexture);
    SDL_DestroyTexture(rightScoreTexture);
    SDL_FreeSurface(leftScoreSurface);
    SDL_FreeSurface(rightScoreSurface);
    SDL_FreeSurface(textSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
