#include "../include/HashTable.h"
#include "DynamicArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTableNode
{
    void* key;
    int   keySize;

    void* value;
    int   valueSize;
} HashTableNode;

struct HashTable
{
    int size;
    int count;
    int (*hashFn)(void*, int, int);
    DynamicArray* entries[1];
};

struct HashTableIter
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
};

HashTable* htNew(int size, int (*hashFn)(void*, int, int))
{
    assert(size > 0);

    HashTable* table = malloc(sizeof(HashTable) + sizeof(HashTableNode) * (size - 1));
    table->size = size;
    table->count = 0;
    table->hashFn = hashFn ? hashFn : &htHash;
    
    for (int i = 0; i < size; i++)
    {
        table->entries[i] = NULL;
    }

    return table;
}

void htFree(HashTable* table)
{
    for (int i = 0, n = table->size; i < n; i++)
    {
        DynamicArray* entry = table->entries[i];
        if (entry)
        {
            for (int j = 0, m = entry->count; j < m; j++)
            {
                HashTableNode* node;
                daGet(entry, j, &node);

                if (node)
                {
                    free(node->value);
                    free(node->key);
                    free(node);
                }
            }

            daFree(entry);
        }
    }

    free(table);
}

void htRemove(HashTable* table, void* key, int keySize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    DynamicArray* entry = table->entries[entryIndex];
    if (entry)
    {
        for (int i = 0, n = entry->count; i < n; i++)
        {
            HashTableNode* node;
            daGet(entry, i, &node);

            if (node && node->keySize == keySize && memcmp(key, node->key, keySize) == 0)
            {
                if (i < entry->count - 1)
                {
                    HashTableNode* lastNode;
                    daGet(entry, entry->count - 1, &lastNode);
                    daSet(entry, i, &lastNode);
                }

                table->count--;
                entry->count--;
                
                free(node->value);
                free(node->key);
                free(node);
            }
        }
    }
}

void* htSearch(HashTable* table, void* key, int keySize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    DynamicArray* entry = table->entries[entryIndex];
    if (entry)
    {
        for (int i = 0, n = entry->count; i < n; i++)
        {
            HashTableNode* node;
            daGet(entry, i, &node);

            if (node->keySize == keySize && memcmp(key, node->key, keySize) == 0)
            {
                return node->value;
            }
        }
    }

    return NULL;
}

void* htInsert(HashTable* table, void* key, int keySize, void* value, int valueSize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    DynamicArray* entry = table->entries[entryIndex];

    HashTableNode* currNode = NULL;
    if (!entry)
    {
        table->entries[entryIndex] = entry = daNew(8, sizeof(HashTableNode*));
        if (!entry)
        {
            return NULL;
        }
    }
    else
    {
        for (int i = 0, n = entry->count; i < n; i++)
        {
            HashTableNode* node;
            daGet(entry, i, &node);

            if (node->keySize == keySize && memcmp(key, node->key, keySize) == 0)
            {
                currNode = node;
                break;
            }
        }
    }

    if (!currNode)
    {
        currNode = malloc(sizeof(HashTableNode));
        currNode->key = malloc(keySize);
        currNode->keySize = keySize; 
        currNode->value = NULL;
        currNode->valueSize = 0;
        memcpy(currNode->key, key, keySize);

        daPush(entry, &currNode);
        table->count++;
    }

    if (!currNode->value || currNode->valueSize != valueSize)
    {
        free(currNode->value);

        currNode->value = malloc(valueSize);
        currNode->valueSize = valueSize;
    }

    memcpy(currNode->value, value, valueSize);
    return currNode->value;
}

int htHash(void* key, int keySize, int tableSize)
{
    assert(tableSize > 0);

    int sum = 0;
    for (int i = 0; i < keySize; i++)
    {
        sum += ((unsigned char*)key)[i] * (i + 1);
    }

    return (sum % tableSize);
}

HashTableIter* htIterNew(HashTable* table)
{
    HashTableIter* iter = malloc(sizeof(*iter));
    iter->internal.table = table;
    iter->internal.entry = NULL;
    iter->internal.index = -1;
    iter->internal.entryIndex = -1;

    return iter;
}

void htIterFree(HashTableIter* iter)
{
    free(iter);
}

int htIterNext(HashTableIter* iter)
{
    HashTable* table = iter->internal.table;

    int entryIndex = iter->internal.entryIndex;
    if (iter->internal.entry == NULL || iter->internal.index < 0 || iter->internal.index >= iter->internal.entry->count - 1)
    {
        entryIndex++;
        while (iter->internal.table->entries[entryIndex] == NULL && entryIndex < table->size)
        {
            entryIndex++;
        }

        if (entryIndex >= table->size)
        {
            iter->internal.entry = NULL;
            iter->internal.entryIndex = entryIndex;

            iter->key = NULL;
            iter->keySize = 0;

            iter->value = NULL;
            iter->valueSize = 0;

            return 0;
        }
        else
        {
            iter->internal.index = 0;
            iter->internal.entry = table->entries[entryIndex];
            iter->internal.entryIndex = entryIndex;
        }
    }
    else
    {
        iter->internal.index++;
    }
    
    HashTableNode* node;
    daGet(iter->internal.entry, iter->internal.index, &node);

    iter->key = node->key;
    iter->value = node->value;
    iter->keySize = node->keySize;
    iter->valueSize = node->valueSize;

    return 1;
}

void* htIterGetKey(HashTableIter* iter)
{
    return iter->key;
}

void* htIterGetValue(HashTableIter* iter)
{
    return iter->value;
}
