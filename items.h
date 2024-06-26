/*

Essa header possui os itens que serão implementados no jogo

*/

#ifndef ITEMS
#define ITEMS
#include "imports.h"

typedef struct Sword
{
    int id;
    int damage;
    int durability;
    int material;
    wchar_t * img; 
} Sword;

typedef struct Pickaxe
{
    int id;
    int damage;
    int durability;
    int material;
    int speed;
} Pickaxe;

typedef struct Shotgun
{
    int id;
    int damage;
    int durability;
    int material;
    int ammo;
} Shotgun;


#endif