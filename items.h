/*

Essa header possui os itens que serão implementados no jogo

*/

#ifndef ITEMS
#define ITEMS
#include "imports.h"

typedef struct Item
{
    int id;
    int damage;
    int durability;
    int ammo;
    wchar_t * img;
} Item;

void itemDefine(Item * item)
{
    item->durability = 64;
    switch (item->id)
    {
    case 0:
        item->damage = 0;
        item->ammo = -1;
        item->img = L"imagens/PicaretaMadeira.bmp";
        break;

    case 1:
        item->damage = 2;
        item->ammo = -1;
        item->img = L"imagens/EspadaMadeira.bmp";
        break;
    
    case 2:
        item->damage = 5;
        item->ammo = 10;
        item->img = L"imagens/Shotgun.bmp";
        break;
    
    default:
        break;
    }
}

#endif