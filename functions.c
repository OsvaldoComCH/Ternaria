#include "imports.h"

HWND Ghwnd;

int zombieGravity = 0;

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
            //writeArchive(&Map);
            return 1;
        }
        N = N->Next;
    }
    return 0;
}

int PlaceBlocks()
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
            return 0;
        }
        N = N->Next;
    }
    block * B = malloc(sizeof(block));
    B->x = Mouse.x / 32;
    B->y = (675 - Mouse.y) / 32;
    B->type = 1;
    blockDefine(B);
    LListAdd(&Map, B);
    //writeArchive(&Map);
}

void moveDownZombie(zombie * zombie, int pixels)
{
    zombie->hitbox.top += pixels;
    zombie->hitbox.bottom += pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombieGravity = 0;
        zombie->hitbox.bottom = B->hitbox.top;
        zombie->hitbox.top = zombie->hitbox.bottom - 63;
    }
}

void moveUpZombie(zombie * zombie, int pixels)
{
    zombie->hitbox.top -= pixels;
    zombie->hitbox.bottom -= pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombieGravity = 0;
        zombie->hitbox.top = B->hitbox.bottom;
        zombie->hitbox.bottom = zombie->hitbox.top + 63;
    }
}

void moveRightZombie(zombie * zombie, int pixels)
{
    zombie->state = zombie->state = 1;
    zombie->hitbox.left += pixels;
    zombie->hitbox.right += pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombie->hitbox.right = B->hitbox.left;
        zombie->hitbox.left = zombie->hitbox.right - 31;
        moveUpZombie(zombie, 16);
    }
}

void moveLeftZombie(zombie * zombie, int pixels)
{
    zombie->state = zombie->state = 0;
    zombie->hitbox.left -= pixels;
    zombie->hitbox.right -= pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombie->hitbox.left = B->hitbox.right;
        zombie->hitbox.right = zombie->hitbox.left + 31;
        moveUpZombie(zombie, 16);
    }

}

void ZombieGravity(zombie * zombie)
{
    moveDownZombie(zombie, zombieGravity);
    if(zombieGravity < 20)
    {
        zombieGravity++;
    }
}

void moveZombie(character * player, zombie * zombie)
{
    int right = player->hitbox.right;
    int left = player->hitbox.left;
    ZombieGravity(zombie);
    if (zombie->hitbox.right < right)
    {
        moveRightZombie(zombie, 2);
    }
    if (zombie->hitbox.left > left)
    {
        moveLeftZombie(zombie, 2);
    }
}