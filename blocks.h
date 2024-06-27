#ifndef BLOCKS
#define BLOCKS
#include <stdio.h>
#include "imports.h"


// Estrutura dos blocos, especificando com posição e tipo.
typedef struct block
{
    RECT hitbox;
    int x;
    int y;
    int type;
    wchar_t * img;
} block;

// Função de definição da textura e colisão dos blocos
void blockDefine(block * B)
{
    B->hitbox.left = B->x*32;
    B->hitbox.top = 649 - (B->y*32);
    B->hitbox.right = B->hitbox.left + 32;
    B->hitbox.bottom = B->hitbox.top + 32;
    switch (B->type)
    {
    case 1:
        B->img = L"imagens/grama.bmp";
        break;

    case 2:
        B->img = L"imagens/terra.bmp";
        break;
        
    default:
        break;
    }
}
/* 
Função de leitura do arquivo de mapa, fazemos a construção do mapa do Ternaria por meio da leitura desse arquivo.
*/ 
void readArchive(DArray *lista)
{
    int count = 0;
    FILE * File = fopen("Map.txt", "r");

    while(!feof(File))
    {
        ++count;
        block *bloco = malloc(sizeof(block));
        fscanf(File, "%i,%i,%i", &bloco->x, &bloco->y, &bloco->type);
        blockDefine(bloco);
        DArrayAdd(lista, bloco);
    }
    fclose(File);
}

void writeArchive(DArray *lista)
{
    int count = 0;
    FILE * File = fopen("Map.txt", "w");
    for(int i = 0; i < lista->Size; ++i)
    {
        block * B = (block *) lista->List[i];
        fprintf(File, "%i,%i,%i\n", B->x, B->y, B->type);
    }
    fclose(File);
}
#endif
