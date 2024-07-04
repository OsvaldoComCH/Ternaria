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
    Item inventory[9];
    int mainSlot;
    int life;
    int damage;
    int vulnerability;
    int state;
    int facing;
    int canJump;
    int gravity;
    int knockback;
    int knockbackSide;
    int canMove;
    int run;
} character;

/*
Estrutura do zumbi.
*/

typedef struct zombie
{
    RECT hitbox;
    int life;
    int baseLife;
    int damage;
    int state;
    int facing;
    int canJump;
    int knockback;
    int knockbackSide;
    int canMove;
    int gravity;
    int respawn;
} zombie;

#endif
