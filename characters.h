#ifndef CHARACTERS
#define CHARACTERS
#include "imports.h"
/* 
Estrutura dos personagens, especificando posição e subposição.
A subposição serve para calcular colisões entre personagem e bloco.
*/

typedef struct inventory
{
    Item slot;
    int mainSlot;
    int quantity;
} inventory;

typedef struct character
{
    RECT hitbox;
    inventory inventory[9];
    int life;
    int damage;
    int state;
    int facing;
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
} zombie;

#endif
