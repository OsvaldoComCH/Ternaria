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
    wchar_t * img;
    R.top = Player->hitbox.top + 24;
    R.bottom = R.top + 15;
    if(Player->state)
    {
        if(Player->facing == 1)
        {
            R.left = Player->hitbox.left;
            R.right = R.left + 15;
            img = Player->inventory[Player->mainSlot].imgLeft;
        }else
        {
            R.left = Player->hitbox.left + 16;
            R.right = R.left + 15;
            img = Player->inventory[Player->mainSlot].imgRight;
        }
    }else
    {
        if(Player->facing == 1)
        {
            img = Player->inventory[Player->mainSlot].imgLeft;
        }else
        {
            img = Player->inventory[Player->mainSlot].imgRight;
        }
        R.left = Player->hitbox.left + 8;
        R.right = R.left + 15;
    }

    DrawImg(hdc, &R, img);
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

void renderLife(HDC hdc, int life)
{
    wchar_t * img1;
    wchar_t * img2;
    int left = 5, top = 50, right = 37, bottom = 82;
    for(int i = 1; i <= life; i ++)
    {
        img1 = L"imagens/Vida.bmp";
        RECT R1 = {left, top, right, bottom};
        DrawImg(hdc, &R1, img1);
        left += 20;
        right += 20;
    }
    for(int i = life + 1; i <= 10; i++)
    {
        img1 = L"imagens/BrokenHeart.bmp";
        RECT R1 = {left, top, right, bottom};
        DrawImg(hdc, &R1, img1);
        left += 20;
        right += 20;
    }
}
