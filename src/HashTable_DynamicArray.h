#pragma once

#include "DynamicArray.h"

typedef struct HashTableNode
{
    void* key;
    int   keySize;

    void* value;
    int   valueSize;
} HashTableNode;

typedef struct HashTable
{
    int size;
    int count;
    int (*hashFn)(void*, int, int);
    DynamicArray* entries[1];
} HashTable;

typedef struct HashTableIter
{
    void* key;
    int   keySize;
    void* value;
    int   valueSize;

    struct
    {
        HashTable*      table;
        DynamicArray*   entry;
        int             index;
        int             entryIndex;
    } internal;
} HashTableIter;

HashTable*  htNew(int size, int (*hashFn)(void*, int, int));
void        htFree(HashTable* table);

void        htRemove(HashTable* table, void* key, int keySize);
void*       htSearch(HashTable* table, void* key, int keySize);
void*       htInsert(HashTable* talbe, void* key, int keySize, void* value, int valueSize);

int         htHash(void* key, int keySize, int tableSize);

void        htIteration(HashTable* table, HashTableIter* iter);
int         htNext(HashTableIter* iter);

#define dictRemove(table, key)                      htRemove(table, key, strlen(key) + 1)
#define dictSearch(table, key)         (const char*)htSearch(table, key, strlen(key) + 1)
#define dictInsert(table, key, value)  (const char*)htInsert(table, key, strlen(key) + 1, value, strlen(value) + 1)
