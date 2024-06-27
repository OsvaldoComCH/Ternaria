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
        B->img = L"imagens/Grama.bmp";
        break;

    case 2:
        B->img = L"imagens/Terra.bmp";
        break;
        
    default:
        break;
    }
}

void createArchive()
{
    FILE * map = fopen("Map.txt", "w");
    int camada = rand()%6 + 2;
    
    for(int x = 0; x < 30; x++)
    {
        for(int y = camada; y >= 0; y--)
        {
            fprintf(map, "%d,%d,%d\n", x, y, (y == camada) ? 1 : 2);
        }
        camada += rand()%3 - 1;
        if(camada < 2)
        {
            camada = 2;
        }
        if(camada > 7)
        {
            camada = 7;
        }
    }
    fclose(map);
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
