#include "imports.h"

HWND Ghwnd;

//Desenha a imagem
void DrawImg(HDC hdc, const RECT * Rect, const wchar_t * ImgPath)
{
    BITMAP bm;
    HBITMAP Image = (HBITMAP)LoadImage(NULL, ImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    TransparentBlt(hdc, Rect->left, Rect->top, bm.bmWidth, bm.bmHeight, BitmapDC,
    0, 0, Rect->right - Rect->left, Rect->bottom - Rect->top, RGB(255, 0, 255));
    DeleteDC(BitmapDC);
    DeleteObject(Image);
}

void DrawRect(HDC hdc, const RECT * Rect, COLORREF Color)
{
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, Color);
    Rectangle(hdc, Rect->left, Rect->top, Rect->right, Rect->bottom);
}

int DestroyBlocks()
{
    LLNode * N = Map.Head;
    POINT Mouse;
    RECT WindowRect;
    GetWindowRect(Ghwnd, &WindowRect);
    if(!GetCursorPos(&Mouse))
    {
        return -1;
    }
    ScreenToClient(Ghwnd, &Mouse);
    for(int i = 0; i < Map.Size; ++i)
    {
        block * B = (block *)N->Value;
        if(PtInRect(&B->hitbox, Mouse))
        {
            free(B);
            LListRemove(&Map, i);
            return 1;
        }
        N = N->Next;
    }
    return 0;
}

void moveZombie(RECT * player)
{
    int right = player->right;
    int left = player->left;


}

void zombieJump(zombie * zombie, int pixels)
{
    block * b = MapCollision(&zombie->hitbox);
    int C = Collision(&zombie->hitbox, &b->hitbox);
    if (C == 2)
    {
        moveUpZombie(zombie, 16);
    }
}