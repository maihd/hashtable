#include "../include/HashTable.h"
#include "DynamicArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTableEntry
{
    int next;
    
    void* key;
    int keySize;

    void* value;
    int valueSize;
} HashTableEntry;

typedef struct HashTable
{
    int size;
    int (*hashFn)(void*, int, int);

    int* hashs;
    DynamicArray* entries;
} HashTable;

struct HashTableIter
{
    HashTable*  table;
    int         index;
};

HashTable* htNew(int size, int (*hashFn)(void*, int, int))
{
    assert(size > 0);

    HashTable* table = malloc(sizeof(HashTable));
    table->size = size;
    table->hashFn = hashFn ? hashFn : &htHash;
    
    table->hashs = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        table->hashs[i] = -1;
    }

    table->entries = daNew(size, sizeof(HashTableEntry));

    return table;
}

void htFree(HashTable* table)
{
    for (int i = 0, n = table->size; i < n; i++)
    {
        HashTableEntry entry;
        daGet(table->entries, i, &entry);
        
        free(entry.value);
        free(entry.key);
    }

    daFree(table->entries);
    free(table->hashs);
    free(table);
}

int indexOf(HashTable* table, void* key, int keySize, int* outHash, int* outPrev)
{
    if (!table->entries->count <= 0)
    {
        return -1;
    }

    int hash = table->hashFn(key, keySize, table->size);
    int curr = table->hashs[hash];
    int prev = -1;

    while (curr > -1)
    {
        HashTableEntry entry;
        daGet(table->entries, curr, &entry);

        if (memcmp(entry.key, key, keySize) == 0)
        {
            break;
        }

        prev = curr;
        curr = entry.next;
    }

    if (outHash) *outHash = hash;
    if (outPrev) *outPrev = prev;
    return curr;
}

void htRemove(HashTable* table, void* key, int keySize)
{
    int prev;
    int hash;
    int curr = indexOf(table, key, keySize, &hash, &prev);
    if (curr > -1)
    {
        HashTableEntry entry;
        daGet(table->entries, curr, &entry);
        free(entry.value);
        free(entry.key);

        if (curr < table->entries->count - 1)
        {
            int lastIndex = table->entries->count - 1;
            HashTableEntry lastEntry;
            daGet(table->entries, lastIndex, &lastEntry);
            daSet(table->entries, curr, &lastEntry);

            indexOf(table, lastEntry.key, lastEntry.keySize, &hash, &prev);
            if (prev > -1)
            {
                HashTableEntry prevEntry;
                daGet(table->entries, prev, &prevEntry);
                prevEntry.next = curr;
                daSet(table->entries, prev, &prevEntry);
            }
            else
            {
                table->hashs[hash] = curr;
            }
        }
        else
        {
            if (prev > -1)
            {
                HashTableEntry prevEntry;
                daGet(table->entries, prev, &prevEntry);
                prevEntry.next = -1;
                daSet(table->entries, prev, &prevEntry);
            }
            else
            {
                table->hashs[hash] = -1;
            }
        }

        table->entries->count--;
    }
}

void* htSearch(HashTable* table, void* key, int keySize)
{
    int curr = indexOf(table, key, keySize, NULL, NULL);
    if (curr > -1)
    {
        HashTableEntry entry;
        daGet(table->entries, curr, &entry);
        return entry.value;
    }

    return NULL;
}

void* htInsert(HashTable* table, void* key, int keySize, void* value, int valueSize)
{
    int curr = indexOf(table, key, keySize, NULL, NULL);
    if (curr > -1)
    {
    }
    else
    {
        
    }
    return NULL;
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
    iter->table = table;
    iter->index = -1;

    return iter;
}

void htIterFree(HashTableIter* iter)
{
    free(iter);
}

int htNext(HashTableIter* iter)
{
    if (iter->index < iter->table->entries->count)
    {
        iter->index++;
        return 1;
    }

    return 0;
}

void* htIterGetKey(HashTableIter* iter)
{
    HashTableEntry entry;
    daGet(iter->table->entries, iter->index, &entry);
    return entry.key;
}

void* htIterGetValue(HashTableIter* iter)
{
    HashTableEntry entry;
    daGet(iter->table->entries, iter->index, &entry);
    return entry.value;
}
