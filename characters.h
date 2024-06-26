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
    int jump;
    int canJump;
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
    wchar_t * img;
} zombie;

#endif
