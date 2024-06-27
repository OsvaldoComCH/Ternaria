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


void readItems(character * Player)
{
    int count = 0;
    FILE * ItemFile = fopen("items.txt", "r");

    while (!feof(ItemFile) && count < 9)
    {
        fscanf(ItemFile, "%i, %i, %i, %i", &Player->inventory[count].id, &Player->inventory[count].damage,
        &Player->inventory[count].durability, &Player->inventory[count].ammo);
        
        itemDefine(&Player->inventory[count]);
        ++count;
    }
    fclose(ItemFile);
}

#endif
