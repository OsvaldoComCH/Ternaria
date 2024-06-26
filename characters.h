#ifndef CHARACTERS
#define CHARACTERS
#include "imports.h"
/* 
Estrutura dos personagens, especificando posição e subposição.
A subposição serve para calcular colisões entre personagem e bloco.
*/

typedef struct character
{
    RECT hitbox;
    int inventory[32];
    int life;
    int damage;
    int state;
} character;

/*
Estrutura do zumbi.
*/

typedef struct zombie
{
    RECT hitbox;
    int life;
    int damage;
    int state;
    int jumpBot;
    int canJumpBot;
    wchar_t * img;
} zombie;

// void moveZombie(RECT * player)
// {
//     int right = player->right;
//     int left = player->left;


// }

// void zombieJump(zombie * zombie, int pixels)
// {
//     block * b = MapCollision(&zombie->hitbox);
//     int C = Collision(&zombie->hitbox, &b->hitbox);
//     if (C == 2)
//     {
//         moveUpZombie(zombie, 16);
//     }
// }

#endif
