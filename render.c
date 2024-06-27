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
        if (Player->facing == 1)
        {
            img = L"imagens/Ferzinho3.bmp";
        } else {
            img = L"imagens/Ferzinho3DireitaParado.bmp";
        }
    }
    else
    {
        if (Player->facing == 1)
        {
            img = L"imagens/Ferzinho3Andante.bmp";
        } else {
            img = L"imagens/FerzinhoDireita.bmp";
        }
    }
    DrawImg(hdc, &Player->hitbox, img);
}

void RenderZombie(const zombie * zombie, HDC hdc)
{
    wchar_t * img;
    if(zombie->state == 0)
    {
        img = L"imagens/Zumbi.bmp";
    }
    else
    {
        img = L"imagens/ZumbiDireita.bmp";
    }
    DrawImg(hdc, &zombie->hitbox, img);
}

void renderInv(HDC hdc)
{
    wchar_t * img = L"BarraInv.bmp";
    RECT rect = {100, 100, 288, 32};
    DrawImg(hdc, &rect, img);
}

void renderLife(HDC hdc)
{
    wchar_t * img = L"vida.bmp";
    RECT rect = {30, 30, 16, 16};
    DrawImg(hdc, &rect, img);
}
