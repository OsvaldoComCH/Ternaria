#include "imports.h"
#include "functions.c"

void RenderMap(const LList * Map, HDC hdc)
{
    LLNode * N = Map->Head;
    for(int i = 0; i < Map->Size; ++i)
    {
        block * B = (block *) N->Value;
        DrawImg(hdc, &B->hitbox, B->img);
        N = N->Next;
    }
}

void RenderPlayer(const character * Player, HDC hdc)
{
    wchar_t * img;
    if(Player->state == 0)
    {
        img = L"imagens/Ferzinho3.bmp";
    }
    else
    {
        img = L"imagens/Ferzinho3Andante.bmp";
    }
    DrawImg(hdc, &Player->hitbox, img);
}
