#include "imports.h"

HWND Ghwnd;


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
    block * B = malloc(sizeof(block));
    B->x = Mouse.x / 32;
    B->y = (675 - Mouse.y) / 32;
    B->type = 2;
    int canPlace = 0;
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
            canPlace = 1;
        }
        B->hitbox.right -= 32;
        B->hitbox.left += 32;
        B->hitbox.bottom += 32;
        B->hitbox.top -= 32;
        if(Collision(&B->hitbox, &C->hitbox))
        {
            canPlace = 1;
        }
        B->hitbox.bottom -= 32;
        B->hitbox.top += 32;
        if(Collision(&B->hitbox, &C->hitbox))
        {
            canPlace = 0;
            break;
        }
    }
    if(!canPlace)
    {
        free(B);
        return 0;
    }
    DArrayAdd(&Map, B);
    WriteArchive(&Map);
    return 1;
}

void SpawnZombie(zombie * Zombie, const character * Player)
{
    int PlayerX = Player->hitbox.left >> 5;
    
    Zombie->hitbox.left = ((PlayerX + 15) % 30) * 32;
    Zombie->hitbox.right = Zombie->hitbox.left + 31;
    Zombie->hitbox.top = 512;
    Zombie->hitbox.bottom = Zombie->hitbox.top + 63;
    while(MapCollision(&Zombie->hitbox))
    {
        Zombie->hitbox.top -= 32;
        Zombie->hitbox.bottom -= 32;
    }
    Zombie->life = 12;
    Zombie->damage = 2;
    Zombie->canJump = 1;
    Zombie->state = 1;
    Zombie->knockback = 0;
    Zombie->canMove = 1;
}

void SpawnPlayer(character * Player)
{
    Player->hitbox.left = 320;
    Player->hitbox.right = Player->hitbox.left + 31;
    Player->hitbox.top = 512;
    Player->hitbox.bottom = 700;
    if(!MapCollision(&Player->hitbox))
    {
        Player->hitbox.bottom = Player->hitbox.top + 63;
        block * B = malloc(sizeof(block));
        B->type = 1;
        B->x = Player->hitbox.left >> 5;
        B->y = ((649 - Player->hitbox.bottom) >> 5) + 1;
        BlockDefine(B);
        DArrayAdd(&Map, B);
    }
    Player->hitbox.bottom = Player->hitbox.top + 63;
    while(MapCollision(&Player->hitbox))
    {
        Player->hitbox.top -= 32;
        Player->hitbox.bottom -= 32;
    }
    Player->life = 10;
    Player->damage = 1;
    Player->state = 0;
    Player->mainSlot = 0;
    Player->inventory[0].id = 0;
    Player->inventory[1].id = 1;
    Player->inventory[2].id = 2;
    Player->inventory[3].id = 3;
    ItemDefine(&Player->inventory[0]);
    ItemDefine(&Player->inventory[1]);
    ItemDefine(&Player->inventory[2]);
    ItemDefine(&Player->inventory[3]);
    Player->gravity = 0;
    Player->canJump = 1;
    Player->knockback = 0;
    Player->knockbackSide = 0;
    Player->canMove = 1;
}

// Move o zumbi para baixo
void MoveDownZombie(zombie * zombie, int pixels)
{
    zombie->hitbox.top += pixels;
    zombie->hitbox.bottom += pixels;
    block * B = MapCollision(&zombie->hitbox);
    if(B != NULL)
    {
        zombie->gravity = 0;
        zombie->canJump = 1;
        zombie->hitbox.bottom = B->hitbox.top;
        zombie->hitbox.top = zombie->hitbox.bottom - 63;
        zombie->canMove = 1;
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
        zombie->gravity = 0;
        zombie->canJump = 0;
        zombie->hitbox.top = B->hitbox.bottom;
        zombie->hitbox.bottom = zombie->hitbox.top + 63;
    }
}

//Pulo do zumbi
void ZombieJump(zombie * Zombie, int Pixels) 
{
    if(Zombie->canJump == 1)
    {
        if(Zombie->gravity == 15)
        {
            Zombie->canJump = 0;
            Zombie->gravity = 0;
            return;
        }
        MoveUpZombie(Zombie, Pixels);
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
        if(zombie->canJump)
        {
            ZombieJump(zombie, 15);
        }
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
        if(zombie->canJump)
        {
            ZombieJump(zombie, 15);
        }
    }

}

// O zumbi sofre gravidade
void ZombieGravity(zombie * zombie)
{
    MoveDownZombie(zombie, zombie->gravity);
    if(zombie->gravity < 20)
    {
        zombie->gravity++;
    }
}

// Move o zumbi até o jogador, caso a hitbox do zumbi colida com a do player, o player toma dano
void KnockbackZombie(zombie * zombie)
{
    if(zombie->knockbackSide == 1)
    {
        MoveLeftZombie(zombie, 7);
    } else {
        MoveRightZombie(zombie, 7);
    }
    MoveUpZombie(zombie, 12);
    ZombieGravity(zombie);
}

void MoveZombie(character * player, zombie * zombie)
{
    if(zombie->respawn)
    {
        --zombie->respawn;
        if(!zombie->respawn)
        {
            SpawnZombie(zombie, player);
        }
        return;
    }
    if(zombie->knockback)
    {
        KnockbackZombie(zombie);
        --zombie->knockback;
        zombie->canMove = 0;
        return;
    }
    if(zombie->canMove)
    {
        int left = player->hitbox.left;
        int right = player->hitbox.right;
        if(zombie->hitbox.right < right)
        {
            MoveRightZombie(zombie, 2);
        }
        if(zombie->hitbox.left > left)
        {
            MoveLeftZombie(zombie, 2);
        }
    }
    ZombieGravity(zombie);
}

void DamageZombie(zombie * Zombie, int Damage)
{
    Zombie->life -= Damage;
    if(Zombie->life <= 0)
    {
        MoveUpZombie(Zombie, 800);
        Zombie->respawn = 150;
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
        DamageZombie(Zombie, Player->inventory[Player->mainSlot].damage);
        return (Zombie->hitbox.left + 16) - (Player->hitbox.left + 15);
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
        DamageZombie(Zombie, Player->inventory[Player->mainSlot].damage);
        return (Zombie->hitbox.left + 16) - (Player->hitbox.left + 15);
    }
    return 0;
}