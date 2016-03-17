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

#include "main.h"
#include "SDL2/SDL_ttf.h"
#include "paddles.h"
#include "ball.h"
#include "SDL.h"

// Declare Variables - Shit Needs Cleaned, Mang
char scoreBuffer[10];
const Uint8 *currentKeyState;

// Declare SDL Assets
SDL_Surface *scoreSurface = NULL;
SDL_Texture *scoreTexture = NULL;

// Declare Rects
SDL_Rect startMessageRect;
SDL_Rect divider;
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
    if(currentKeyState[SDL_SCANCODE_RETURN]) screen = 1; // Unpause
    if(currentKeyState[SDL_SCANCODE_ESCAPE]) screen = 2; // Pause
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
    // Query Scoreboard
    sprintf(scoreBuffer, "%d   %d", leftScore, rightScore);
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(silkscreen, scoreBuffer, white); // Get score to surface.
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface); // Make it a texture.
    SDL_FreeSurface(scoreSurface);
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h); // scoreRect now contains the width and height of our scoreboard.
    scoreRect.w *= 4; // Adjust font size manually here from texture size queried.
    scoreRect.h *= 4; // Adjust font size manually here from texture size queried.
    scoreRect.x = SCREEN_WIDTH / 2  - scoreRect.w / 2;
    // Paddle Leash
    if(leftPaddle.y < 0) leftPaddle.y++; // If the left paddle goes above the screen, bring it back down.
    if(leftPaddle.y + leftPaddle.h > SCREEN_HEIGHT) leftPaddle.y--; // If the left paddle goes under the screen, bring it back up.
    if(rightPaddle.y < 0) rightPaddle.y++; // If the right paddle goes above the screen, bring it back down.
    if(rightPaddle.y + leftPaddle.h > SCREEN_HEIGHT) rightPaddle.y--; // If the right paddle goes under the screen, bring it back up.
    // Render Game
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background
    SDL_RenderDrawRect(renderer, &divider); // Divider
    SDL_RenderFillRect(renderer, &divider);
    SDL_RenderDrawRect(renderer, &leftPaddle); // Left Paddle
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderDrawRect(renderer, &rightPaddle); // Right Paddle
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderDrawRect(renderer, &ball); // Ball
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect); // Scoreboard
    SDL_RenderPresent(renderer); // Draw
    SDL_DestroyTexture(scoreTexture); // Free Scoreboard Texture
}

int main(int argc, char* argv[])
{
    initializeSDL();
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

        switch(screen)
        {
            case 0: // Start Menu
                clearScreen();
                renderStartMenu();
                break;
            case 1: // Game
                ballPhysics();
                clearScreen();
                renderGame();
                break;
            case 2: // Pause Menu
                clearScreen();
                SDL_RenderCopy(renderer, startMessageTexture, NULL, &startMessageRect);
                renderGame();
                break;
        }

        SDL_Delay(2); // Slow down the loop.
    }

    quitSDL(); // Quit all SDL subsystems and free memory.
    return 0;
}
