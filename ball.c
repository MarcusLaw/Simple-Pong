#include "SDL.h"
#include "main.h"
#include "ball.h"

int xBallDirection = 1;
int yBallDirection = -1;

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
