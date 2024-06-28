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
    wchar_t * imgLeft;
    wchar_t * imgRight;
} Item;

void ItemDefine(Item * item)
{
    item->durability = 64;
    switch (item->id)
    {
    case 0:
        item->damage = 0;
        item->ammo = -1;
        item->imgLeft = L"imagens/PicaretaEsquerda.bmp";
        item->imgRight = L"imagens/PicaretaDireita.bmp";
        break;

    case 1:
        item->damage = 2;
        item->ammo = -1;
        item->imgLeft = L"imagens/EspadaEsquerda.bmp";
        item->imgRight = L"imagens/EspadaDireita.bmp";
        break;
    
    case 2:
        item->damage = 5;
        item->ammo = 10;
        item->imgLeft = L"imagens/ShotgunEsquerda.bmp";
        item->imgRight = L"imagens/ShotgunDireita.bmp";
        break;
    
    case 3:
        item->damage = 0;
        item->ammo = 50;
        item->imgLeft = L"imagens/Bloquinho.bmp";
        item->imgRight = L"imagens/Bloquinho.bmp";
    
    default:
        break;
    }
}

#endif