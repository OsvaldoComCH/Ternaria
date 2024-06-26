#include "imports.h"
#include "render.c"

int gravity = 0;
int canJump = 1;

void MoveLeft(character * Player, int Pixels)
{
    Player->state = Player->state ^ 1;
    Player->facing = 1;
    Player->hitbox.left -= Pixels;
    Player->hitbox.right -= Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        Player->hitbox.left = B->hitbox.right;
        Player->hitbox.right = Player->hitbox.left + 31;
    }
}

void MoveRight(character * Player, int Pixels)
{
    Player->state = Player->state ^ 1;
    Player->facing = 2;
    Player->hitbox.left += Pixels;
    Player->hitbox.right += Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        Player->hitbox.right = B->hitbox.left;
        Player->hitbox.left = Player->hitbox.right - 31;
    }
}

void MoveDown(character * Player, int Pixels)
{
    Player->hitbox.top += Pixels;
    Player->hitbox.bottom += Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        gravity = 0;
        canJump = 1;
        Player->hitbox.bottom = B->hitbox.top;
        Player->hitbox.top = Player->hitbox.bottom - 63;
    }
}

void MoveUp(character * Player, int Pixels)
{
    Player->hitbox.top -= Pixels;
    Player->hitbox.bottom -= Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        canJump = 0;
        gravity = 0;
        Player->hitbox.top = B->hitbox.bottom;
        Player->hitbox.bottom = Player->hitbox.top + 63;
    }
}

void Jump(character * Player, int Pixels) 
{
    if(canJump == 1)
    {
        if(gravity == 15)
        {
            canJump = 0;
            gravity = 0;
            return;
        }
        MoveUp(Player, Pixels);
    }
}

void Gravity(character * Player)
{
    MoveDown(Player, gravity);
    if(gravity < 20)
    {
        gravity++;
    }
}

void input(character * Player, LList * Map)
{
    if(GetAsyncKeyState(VK_RBUTTON))
    {
        DestroyBlocks();
    }
    if(GetAsyncKeyState(VK_A))
    {
        MoveLeft(Player, 5);
    }
    if(GetAsyncKeyState(VK_D))
    {
        MoveRight(Player, 5);
    }
    if(GetAsyncKeyState(VK_SPACE))
    {
        Jump(Player, 15);
    }
    else if(canJump == 1)
    {
        canJump = 0;
        gravity = 0;
    }
    Gravity(Player);
}