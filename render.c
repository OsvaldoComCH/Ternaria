#include "imports.h"
#include "functions.c"

void RenderBkgd(HDC hdc)
{
    BITMAP bm;
    HBITMAP Image = (HBITMAP)LoadImage(NULL, L"imagens/BackGround.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, BitmapDC, 0, 0, SRCCOPY);
    DeleteDC(BitmapDC);
    DeleteObject(Image);
}

void RenderMap(const DArray * Map, HDC hdc)
{
    for(int i = 0; i < Map->Size; ++i)
    {
        block * B = (block *) Map->List[i];
        DrawImg(hdc, &B->hitbox, B->img);
    }
}

void RenderTool(const character * Player, HDC hdc)
{
    RECT R;
    R.top = Player->hitbox.top + 24;
    R.left = Player->hitbox.left + 8;
    R.bottom = R.top + 15;
    R.right = R.left + 15;
    DrawImg(hdc, &R, Player->inventory[Player->mainSlot].img);
}

void RenderPlayer(const character * Player, HDC hdc)
{
    wchar_t * img;
    if(Player->state == 0)
    {
        if (Player->facing == 1)
        {
            img = L"imagens/Fer.bmp";
        } else {
            img = L"imagens/FerDireita.bmp";
        }
    }
    else
    {
        if (Player->facing == 1)
        {
            img = L"imagens/FerAndante.bmp";
        } else {
            img = L"imagens/FerDireitaAndante.bmp";
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
    wchar_t * img = L"imagens/BarraInv.bmp";
    RECT rect = {10, 10, 317, 42};
    DrawImg(hdc, &rect, img);
}

void renderLife(HDC hdc)
{
    wchar_t * img = L"imagens/vida.bmp";
    RECT rect = {30, 30, 16, 16};
    DrawImg(hdc, &rect, img);
}
