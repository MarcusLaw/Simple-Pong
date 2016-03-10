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
int start;
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

    // Declare Rects
        // Start Message
    SDL_Rect startMessage;
    startMessage.w = 480;
    startMessage.h = 64;
    startMessage.x = SCREEN_WIDTH / 2 + startMessage.w / 24;
    startMessage.y = SCREEN_HEIGHT / 4 - startMessage.h / 2;
        //Divider
    SDL_Rect divider;
    divider.w = 1;
    divider.h = SCREEN_HEIGHT;
    divider.x = SCREEN_WIDTH / 2;
    divider.y = 0;
        // Ball
    SDL_Rect ball;
    ball.w = 10;
    ball.h = 10;
    ball.x = SCREEN_WIDTH / 2 - ball.w / 2;
    ball.y = SCREEN_HEIGHT / 2 - ball.h / 2;
        // Left Paddle
    SDL_Rect leftPaddle;
    leftPaddle.w = 10;
    leftPaddle.h = 100;
    leftPaddle.x = 5;
    leftPaddle.y = SCREEN_HEIGHT / 2 - leftPaddle.h / 2;
        // Right Paddle
    SDL_Rect rightPaddle;
    rightPaddle.w = 10;
    rightPaddle.h = 100;
    rightPaddle.x = SCREEN_WIDTH - rightPaddle.w - 5;
    rightPaddle.y = SCREEN_HEIGHT / 2 - rightPaddle.h / 2;
        // Left Score
    SDL_Rect leftScoreRect;
    leftScoreRect.w = 64;
    leftScoreRect.h = 64;
    leftScoreRect.x = SCREEN_WIDTH / 2 - leftScoreRect.w / 2 - 50;
    leftScoreRect.y = 32;
        // Right Score
    SDL_Rect rightScoreRect;
    rightScoreRect.w = 64;
    rightScoreRect.h = 64;
    rightScoreRect.x = SCREEN_WIDTH / 2 - rightScoreRect.w / 2 + 50;
    rightScoreRect.y = 32;

    // Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
        // Start Message
    SDL_RenderCopy(renderer, startMessageTexture, NULL, &startMessage);
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
        // Update Screen
    SDL_RenderPresent(renderer);

    // Main Game Loop
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = 1;
        }

        while(!start)
        {
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT) quit = 1, start = 1;
            }

            currentKeyState = SDL_GetKeyboardState(NULL);
            if(currentKeyState[SDL_SCANCODE_SPACE]) start = 1;

            SDL_Delay(3);
        }

        ball.x = ball.x + xBallDirection;
        ball.y = ball.y + yBallDirection;

		// Get Keystates
		currentKeyState = SDL_GetKeyboardState(NULL);
		if(currentKeyState[SDL_SCANCODE_W]) leftPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_S]) leftPaddle.y++;
		if(currentKeyState[SDL_SCANCODE_UP]) rightPaddle.y--;
		if(currentKeyState[SDL_SCANCODE_DOWN]) rightPaddle.y++;
		if(currentKeyState[SDL_SCANCODE_SPACE]);

		// Logic
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
            // Balls to the Walls
		if(ball.x < 0) rightScore++, ball.x = SCREEN_WIDTH / 2 - ball.w / 2, xBallDirection = 1;
		if(ball.x + ball.w > SCREEN_WIDTH) leftScore++, ball.x = SCREEN_WIDTH / 2 - ball.w / 2, xBallDirection = -1;
		if(ball.y < 0) yBallDirection = 1;
		if(ball.y + ball.h > SCREEN_HEIGHT) yBallDirection = -1;
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

		// Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
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
            // Update Screen
        SDL_RenderPresent(renderer);

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
