#include "imports.h"

int Collision(const RECT * R1, const RECT * R2)
{
    RECT Inter;
    if(IntersectRect(&Inter, R1, R2))
    {
        if(Inter.right - Inter.left > Inter.bottom - Inter.top)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

block * MapCollision(const RECT * R)
{
    LLNode * N = Map.Head;
    for(int i = 0; i < Map.Size; ++i)
    {
        block * B = N->Value;
        if(Collision(&B->hitbox, R))
        {
            return B;
        }
        N = N->Next;
    }
    return NULL;
}

void MoveLeft(character * Player, int Pixels)
{
    Player->state = Player->state ^ 1;
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
        Player->hitbox.top = B->hitbox.bottom;
        Player->hitbox.bottom = Player->hitbox.top + 63;
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



void input(character * Player, LList * Map)
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
