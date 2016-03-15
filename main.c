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
int screen = 2;
int leftScore;
int rightScore;
char scoreBuffer[10];
int xBallDirection = 1;
int yBallDirection = -1;
const Uint8 *currentKeyState;

// Declare SDL Assets
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *silkscreen = NULL;
SDL_Surface *scoreSurface = NULL;
SDL_Texture *scoreTexture = NULL;
SDL_Surface *titleSurface = NULL;
SDL_Texture *titleTexture = NULL;
SDL_Surface *startButtonSurface = NULL;
SDL_Texture *startButtonTexture = NULL;
SDL_Event e;

// Declare Rects
SDL_Rect startMessageRect;
SDL_Rect divider;
SDL_Rect ball;
SDL_Rect leftPaddle;
SDL_Rect rightPaddle;
SDL_Rect scoreRect;
SDL_Rect titleRect;
SDL_Rect startButtonRect;

void defineRects()
{
    // Start Message
    startMessageRect.w = 240;
    startMessageRect.h = 24;
    startMessageRect.x = SCREEN_WIDTH - startMessageRect.w - 5; // SCREEN_WIDTH - messageWidth = keep on screen; -5 = margin
    startMessageRect.y = SCREEN_HEIGHT - startMessageRect.h; // SCREEN_HEIGHT - messageWidth = keep on screen
    // Score Board
    scoreRect.y = 25;
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
}

void getKeystates()
{
    currentKeyState = SDL_GetKeyboardState(NULL);
    if(currentKeyState[SDL_SCANCODE_W]) leftPaddle.y--; // Left Paddle Up
    if(currentKeyState[SDL_SCANCODE_S]) leftPaddle.y++; // Left Paddle Down
    if(currentKeyState[SDL_SCANCODE_UP]) rightPaddle.y--; // Right Paddle Up
    if(currentKeyState[SDL_SCANCODE_DOWN]) rightPaddle.y++; // Right Paddle Down
    if(currentKeyState[SDL_SCANCODE_RETURN]) screen = 0; // Unpause
    if(currentKeyState[SDL_SCANCODE_ESCAPE]) screen = 1; // Pause
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

void renderStartMenu()
{
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
    SDL_RenderPresent(renderer);
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
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    // Draw
    SDL_RenderPresent(renderer);
}

void quitSDL()
{
    TTF_CloseFont(silkscreen);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(startButtonTexture);
    SDL_FreeSurface(startButtonSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("Simple Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    silkscreen = TTF_OpenFont("fonts/slkscr.ttf", 16);
    SDL_Color white = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(silkscreen, "Press Enter to Start", white);
    SDL_Texture *startMessageTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    // Start Menu
    titleSurface = TTF_RenderText_Solid(silkscreen, "Simple Pong", white);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    titleRect.w = 512;
    titleRect.h = 128;
    titleRect.x = SCREEN_WIDTH / 2 - titleRect.w / 2;
    titleRect.y = 25;
        // Start Button
    startButtonSurface = TTF_RenderText_Solid(silkscreen, "Start", white);
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    startButtonRect.w = 128;
    startButtonRect.h = 48;
    startButtonRect.x = SCREEN_WIDTH / 2 - startButtonRect.w / 2;
    startButtonRect.y = SCREEN_HEIGHT / 2;

    defineRects();

    while(!quit) // Main Loop
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT) quit = 1;
        }

        getKeystates();

        /*
        // Query Scoreboard
        sprintf(scoreBuffer, "%d   %d", leftScore, rightScore);
        scoreSurface = TTF_RenderText_Solid(silkscreen, scoreBuffer, white); // Get score to surface.
        scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface); // Make it a texture.
        SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h); // scoreRect now contains the width and height of our scoreboard.
        scoreRect.w *= 4; // Adjust font size manually here from texture size queried.
        scoreRect.h *= 4; // Adjust font size manually here from texture size queried.
        scoreRect.x = SCREEN_WIDTH / 2  - scoreRect.w / 2;
        SDL_RenderDrawRect(renderer, &scoreRect);
        */

        switch(screen)
        {
            case 0: // Game
                paddleMovement();
                ballPhysics();
                clearScreen();
                renderGame();
                break;
            case 1: // Pause Menu
                clearScreen();
                SDL_RenderCopy(renderer, startMessageTexture, NULL, &startMessageRect);
                renderGame();
                break;
            case 2: // Start Menu
                clearScreen();
                renderStartMenu();
                break;
        }

        SDL_Delay(2); // Slow down the loop.
    }

    quitSDL(); // Quit all SDL subsystems and free memory.
    return 0;
}
