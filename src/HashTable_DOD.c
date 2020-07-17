#include "../include/HashTable.h"

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
    int (*hashFn)(void*, int, int);

    int             count;
    int             capacity;
    HashTableEntry* entries;

    int  hashCount;
    int  hashs[1];
} HashTable;

struct HashTableIter
{
    HashTable*  table;
    int         index;
};

HashTable* htNew(int hashCount, int (*hashFn)(void*, int, int))
{
    assert(hashCount > 0);

    HashTable* table = malloc(sizeof(HashTable) + (hashCount - 1) * sizeof(int));
    table->hashCount = hashCount;
    table->hashFn = hashFn ? hashFn : &htHash;

    for (int i = 0; i < hashCount; i++)
    {
        table->hashs[i] = -1;
    }

    table->count    = 0;
    table->capacity = 0;
    table->entries  = NULL;

    return table;
}

void htFree(HashTable* table)
{
    for (int i = 0, n = table->count; i < n; i++)
    {
        HashTableEntry* entry = &table->entries[i];
        
        free(entry->value);
        free(entry->key);
    }

    free(table->entries);
    free(table);
}

int indexOf(HashTable* table, void* key, int keySize, int* outHash, int* outPrev)
{
    int hash = table->hashFn(key, keySize, table->hashCount);
    int curr = table->hashs[hash];
    int prev = -1;

    while (curr > -1)
    {
        HashTableEntry* entry = &table->entries[curr];
        if (memcmp(entry->key, key, keySize) == 0)
        {
            break;
        }

        prev = curr;
        curr = entry->next;
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
        HashTableEntry entry = table->entries[curr];
        free(entry.value);
        free(entry.key);

        if (curr < table->count - 1)
        {
            table->entries[curr] = table->entries[table->count - 1];

            HashTableEntry lastEntry = table->entries[curr];
            indexOf(table, lastEntry.key, lastEntry.keySize, &hash, &prev);
            if (prev > -1)
            {
                table->entries[prev].next = curr;
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
                table->entries[prev].next = -1;
            }
            else
            {
                table->hashs[hash] = -1;
            }
        }

        table->count--;
    }
}

void* htSearch(HashTable* table, void* key, int keySize)
{
    int curr = indexOf(table, key, keySize, NULL, NULL);
    if (curr > -1)
    {
        return table->entries[curr].value;
    }

    return NULL;
}

void* htInsert(HashTable* table, void* key, int keySize, void* value, int valueSize)
{
    int prev;
    int hash;
    int curr = indexOf(table, key, keySize, &hash, &prev);
    if (curr > -1)
    {
        HashTableEntry* entry = &table->entries[curr];

        if (entry->valueSize != valueSize)
        {
            free(entry->value);

            entry->value = malloc(valueSize);
            entry->valueSize = valueSize;
        }
        memcpy(entry->value, value, valueSize);
        return entry->value;
    }
    else
    {
        curr = table->count;

        HashTableEntry entry;
        entry.next = -1;
        entry.key = malloc(keySize);
        entry.keySize = keySize;
        entry.value = malloc(valueSize);
        entry.valueSize = valueSize;

        memcpy(entry.key, key, keySize);
        memcpy(entry.value, value, valueSize);

        if (table->count + 1 > table->capacity)
        {
            table->capacity = (table->capacity > 0 ? table->capacity : 8) * 2;
            table->entries = realloc(table->entries, table->capacity * sizeof(HashTableEntry));
        }
        table->entries[curr] = entry;

        if (prev > -1)
        {
            table->entries[prev].next = curr;
        }
        else
        {
            table->hashs[hash] = curr;
        }

        table->count++;
        return entry.value;
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

int htIterNext(HashTableIter* iter)
{
    if (iter->index < iter->table->count - 1)
    {
        iter->index++;
        return 1;
    }

    return 0;
}

void* htIterGetKey(HashTableIter* iter)
{
    if (iter->index < iter->table->count)
    {
        return iter->table->entries[iter->index].key;
    }
    else
    {
        return NULL;
    }
}

void* htIterGetValue(HashTableIter* iter)
{
    if (iter->index < iter->table->count)
    {
        return iter->table->entries[iter->index].value;
    }
    else
    {
        return NULL;
    }
}
