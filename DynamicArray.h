#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include <stdlib.h>

typedef struct 
{
    void ** List;
    int Size;
    int MaxSize;
} DArray;

void DArrayCreate(DArray * Array, int MaxSize)
{
    Array->Size = 0;
    Array->MaxSize = MaxSize;
    Array->List = (void **) malloc(MaxSize * sizeof(void *));
}

void DArrayDelete(DArray * Array)
{
    free(Array->List);
    Array->MaxSize = 0;
    Array->Size = 0;
}

#endif