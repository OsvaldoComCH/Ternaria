#include "imports.h"


// typedef struct character
// {
//     int xPos, yPos;
//     int xSubPos, ySubPos;
//     int inventory[32];
//     int life;
//     int damage;
//     int jump;
//     int canJump;
//     int state;
// } character;

// player.jump = 0;
// player.canJump = 1;


void MoveLeft(character * Player, int Pixels)
{
    Player->state = Player->state ^ 1;
    Player->xSubPos -= Pixels;
    if(Player->xSubPos < 0)
    {
        Player->xPos -= Player->xSubPos / 32 + 1;
        Player->xSubPos = 32 - Player->xSubPos % 32;
    }
}

void MoveRight(character * Player, int Pixels)
{
    Player->state = Player->state ^ 1;
    Player->xSubPos += Pixels;
    if(Player->xSubPos > 31)
    {
        Player->xPos += Player->xSubPos / 32;
        Player->xSubPos = Player->xSubPos % 32;
    }
}

void MoveDown(character * Player, int Pixels)
{
    Player->ySubPos -= Pixels;
    if(Player->ySubPos < 0)
    {
        Player->yPos -= Player->ySubPos / 32 + 1;
        Player->ySubPos = 32 - Player->ySubPos % 32;
    }
}

void MoveUp(character * Player, int Pixels)
{
    Player->ySubPos += Pixels;
    if(Player->ySubPos > 31)
    {
        Player->yPos += Player->ySubPos / 32;
        Player->ySubPos = Player->ySubPos % 32;
    }
}

void Jump(character * Player, int Pixels)
{
    if(Player->jump <= 0)
    {
        if(Player->canJump == 1)
        {
            Player->jump = 65;
            Player->canJump = 0;
        }
    }
    else
    {
        Player->jump -= 5;
        MoveUp(Player, 5);
    }
}

void Gravity(character * Player, int Pixels)
{
    MoveDown(Player, Pixels);
}



void input(character * Player)
{
    if(GetAsyncKeyState(VK_A))
    {
        MoveLeft(Player, 5);
    }
    if(GetAsyncKeyState(VK_D))
    {
        MoveRight(Player, 5);
    }
    if(GetAsyncKeyState(VK_W))
    {
        MoveUp(Player, 5);
    }
    if(GetAsyncKeyState(VK_S))
    {
        MoveDown(Player, 5);
    }
    if(GetAsyncKeyState(VK_SPACE))
    {
        Jump(Player, 5);
    }
    if(GetAsyncKeyState(VK_G))
    {
        Gravity(Player, 5);
    }
}