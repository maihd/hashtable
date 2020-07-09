#pragma once

/**
 * Simple HashTable implement with LinkedList entries.
 * 
 * Only copies of keys and values are store in the table, not the originals.
 * 
 * The table does not grow automatically.
 * 
 * This table is not thread-safe.
 */

typedef struct HashTableNode
{
    void* key;
    int   keySize;

    void* value;
    int   valueSize;

    struct HashTableNode* next;    
} HashTableNode;

typedef struct HashTable
{
    int size;
    int count;
    int (*hashFn)(void*, int, int);
    HashTableNode* entries[1];
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
        HashTableNode*  entry;
        int             index;
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
