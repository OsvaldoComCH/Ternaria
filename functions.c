#include "imports.h"

HWND Ghwnd;

int zombieGravity = 0, knockbackSideZombie;

// Apaga o retângulo
void EraseRect(HDC hdc, const RECT * Rect)
{
    BITMAP bm;
    HBITMAP Image = (HBITMAP)LoadImage(NULL, L"imagens/BackGround.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    BitBlt(hdc, Rect->left, Rect->top, Rect->right - Rect->left+1, Rect->bottom - Rect->top+1, BitmapDC, Rect->left, Rect->top, SRCCOPY);
    DeleteDC(BitmapDC);
    DeleteObject(Image);
}

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

// Desenha retângulo
void DrawRect(HDC hdc, const RECT * Rect, COLORREF Color)
{
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, Color);
    Rectangle(hdc, Rect->left, Rect->top, Rect->right, Rect->bottom);
}

// Colisão geral
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

// Função de colisão de blocos
block * MapCollision(const RECT * R)
{
    for(int i = 0; i < Map.Size; ++i)
    {
        block * B = (block *) Map.List[i];
        if(Collision(&B->hitbox, R))
        {
            return B;
        }
    }
    return NULL;
}

// Função de destruir blocos
int DestroyBlocks(POINT Mouse)
{
    for(int i = 0; i < Map.Size; ++i)
    {
        block * B = (block *) Map.List[i];
        if(PtInRect(&B->hitbox, Mouse))
        {
            free(B);
            DArrayRemove(&Map, i);
            WriteArchive(&Map);
            return 1;
        }
    }
    return 0;
}

// Função de colocar blocos, caso não tenha nenhum bloco adjacente a outro, não é possível colocar bloco
int PlaceBlocks(character * Player, zombie * Zombie, POINT Mouse)
{
    for(int i = 0; i < Map.Size; ++i)
    {
        block * B = (block *) Map.List[i];
        if(PtInRect(&B->hitbox, Mouse))
        {
            return 0;
        }
    }
    block * B = malloc(sizeof(block));
    B->x = Mouse.x / 32;
    B->y = (675 - Mouse.y) / 32;
    B->type = 2;
    int podePor = 0;
    BlockDefine(B);
    if(Collision(&B->hitbox, &Player->hitbox) || Collision(&B->hitbox, &Zombie->hitbox))
    {
        free(B);
        return 0;
    }
    for(int i = 0; i < Map.Size; ++i)
    {
        block * C = (block *) Map.List[i];
        B->hitbox.right += 32;
        B->hitbox.left -= 32;
        if(Collision(&B->hitbox, &C->hitbox))
        {
            podePor = 1;
        }
        B->hitbox.right -= 32;
        B->hitbox.left += 32;
        B->hitbox.bottom += 32;
        B->hitbox.top -= 32;
        if(Collision(&B->hitbox, &C->hitbox))
        {
            podePor = 1;
        }
        B->hitbox.bottom -= 32;
        B->hitbox.top += 32;
        if(podePor)
        {
            break;
        }
    }
    if(!podePor)
    {
        free(B);
        return 0;
    }
    DArrayAdd(&Map, B);
    WriteArchive(&Map);
    return 1;
}

// Move o zumbi para baixo
void MoveDownZombie(zombie * zombie, int pixels)
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

// Move o zumbi para cima
void MoveUpZombie(zombie * zombie, int pixels)
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

// Move o zumbi para a direita
void MoveRightZombie(zombie * zombie, int pixels)
{
    zombie->state = zombie->state = 1;
    zombie->hitbox.left += pixels;
    zombie->hitbox.right += pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombie->hitbox.right = B->hitbox.left;
        zombie->hitbox.left = zombie->hitbox.right - 31;
        MoveUpZombie(zombie, 16);
    }
}

// Move o zumbi para a esquerda
void MoveLeftZombie(zombie * zombie, int pixels)
{
    zombie->state = zombie->state = 0;
    zombie->hitbox.left -= pixels;
    zombie->hitbox.right -= pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombie->hitbox.left = B->hitbox.right;
        zombie->hitbox.right = zombie->hitbox.left + 31;
        MoveUpZombie(zombie, 16);
    }

}

// O zumbi sofre gravidade
void ZombieGravity(zombie * zombie)
{
    MoveDownZombie(zombie, zombieGravity);
    if(zombieGravity < 20)
    {
        zombieGravity++;
    }
}

// Move o zumbi até o jogador, caso a hitbox do zumbi colida com a do player, o player toma dano
void KnockbackZombie(zombie * zombie)
{
    if(knockbackSideZombie == 1)
    {
        MoveLeftZombie(zombie, 7);
    } else {
        MoveRightZombie(zombie, 7);
    }
    MoveUpZombie(zombie, 7);
}

void MoveZombie(character * player, zombie * zombie)
{
    if(zombie->knockback)
    {
        KnockbackZombie(zombie);
        --zombie->knockback;
        return;
    }
    int right = player->hitbox.right;
    int left = player->hitbox.left;
    ZombieGravity(zombie);
    if (zombie->hitbox.right < right)
    {
        MoveRightZombie(zombie, 2);
    }
    if (zombie->hitbox.left > left)
    {
        MoveLeftZombie(zombie, 2);
    }
}

//Espadada no zumbi
int Slash(zombie * Zombie, character * Player)
{
    RECT Damage;
    Damage.top = Player->hitbox.top;
    Damage.bottom = Player->hitbox.bottom;
    if(Player->facing == 1)
    {
        Damage.right = Player->hitbox.left;
        Damage.left = Damage.right - 64;
    }else
    {
        Damage.left = Player->hitbox.right;
        Damage.right = Damage.left + 64;
    }
    if(Collision(&Zombie->hitbox, &Damage))
    {
        Zombie->life -= Player->inventory[Player->mainSlot].damage;
        return (Zombie->hitbox.left + 16) - (Damage.left + 32);
    }
    return 0;
}

//Dá tiro de doze
int EstragarVelorio(zombie * Zombie, character * Player, POINT Mouse)
{
    RECT Damage;
    Damage.top = Mouse.y - 16;
    Damage.bottom = Mouse.y + 15;
    Damage.left = Mouse.x - 16;
    Damage.right = Mouse.x + 15;
    if(Collision(&Zombie->hitbox, &Damage))
    {
        Zombie->life -= Player->inventory[Player->mainSlot].damage;
        return (Zombie->hitbox.left + 16) - Mouse.x;
    }
    return 0;
}