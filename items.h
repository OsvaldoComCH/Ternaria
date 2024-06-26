/*

Essa header possui os itens que serão implementados no jogo

*/

#ifndef ITEMS
#define ITEMS
#include "imports.h"

typedef struct Sword
{
    int damage;
    int durability;
    int material;
    wchar_t * img; 
} Sword;

typedef struct Pickaxe
{
    int damage;
    int durability;
    int material;
    int speed;
} Pickaxe;


#endif // 