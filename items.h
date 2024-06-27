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
    switch (item->id)
    {
    case 0:
        item->img = L"PicaretaMadeira.bmp";
        break;

    case 1:
        item->img = L"EspadaMadeira.bmp";
        break;
    
    case 2:
        item->img = L"Shotgun.bmp";
        break;
    
    default:
        break;
    }
}

void readItems(DArray * listItems)
{
    int count = 0;
    FILE * ItemFile = fopen("items.txt", "r");

    while (!feof(ItemFile))
    {
        ++count;
        Item * item = malloc(sizeof(Item));
        fscanf(ItemFile, "%i, %i, %i, %i", &item->id, &item->damage, &item->durability, &item->ammo);
        itemDefine(item);
        DArrayAdd(listItems, item);
    }
    fclose(ItemFile);
}
#endif