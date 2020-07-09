#pragma once

#include <stdbool.h>

typedef struct DynamicArray
{
    int     count;
    int     capacity;
    int     elementSize;
    void*   elements;
} DynamicArray;

DynamicArray*   daNew(int capacity, int elementSize);
void            daFree(DynamicArray* array);

void            daSet(DynamicArray* array, int index, const void* element);
void            daGet(const DynamicArray* array, int index, void* outElement);

void            daPush(DynamicArray* array, const void* element);
void            daPop(DynamicArray* array, void* outElement);

bool            daGrow(DynamicArray* array, int capacity);
void            daClear(DynamicArray* array);

bool            daEnsure(DynamicArray* array, int capacity);
