#include "imports.h"
#include "render.c"

int gravity = 0;
int canJump = 1;

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
