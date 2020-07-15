#include "../include/HashTable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTableNode
{
    void* key;
    int   keySize;

    void* value;
    int   valueSize;

    struct HashTableNode* next;    
} HashTableNode;

struct HashTable
{
    int size;
    int count;
    int (*hashFn)(void*, int, int);
    HashTableNode* entries[1];
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
        HashTableNode*  entry;
        int             index;
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
        HashTableNode* node = table->entries[i];
        while (node)
        {
            HashTableNode* next = node->next;

            free(node->key);
            free(node->value);

            node = next;
        }
    }

    free(table);
}

void htRemove(HashTable* table, void* key, int keySize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    HashTableNode* entry = table->entries[entryIndex];
    if (entry)
    {
        HashTableNode* currNode = entry;
        HashTableNode* prevNode = NULL;
        while (currNode)
        {
            if (currNode->keySize == keySize && memcmp(key, currNode->key, keySize) == 0)
            {
                table->count--;

                free(currNode->key);
                free(currNode->value);

                if (prevNode)
                {
                    prevNode->next = currNode->next;
                }
                else
                {
                    table->entries[entryIndex] = currNode->next;
                }

                free(currNode);
                break;
            }

            prevNode = currNode;
            currNode = currNode->next;
        }
    }
}

void* htSearch(HashTable* table, void* key, int keySize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    HashTableNode* entry = table->entries[entryIndex];
    if (!entry)
    {
        return NULL;
    }

    HashTableNode* node = entry;
    while (node)
    {
        if (node->keySize == keySize && memcmp(key, node->key, keySize) == 0)
        {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

void* htInsert(HashTable* table, void* key, int keySize, void* value, int valueSize)
{
    int entryIndex = table->hashFn(key, keySize, table->size);
    HashTableNode* entry = table->entries[entryIndex];
    if (entry)
    {
        HashTableNode* currNode = entry;
        HashTableNode* prevNode = NULL;
        while (currNode)
        {
            if (currNode->keySize == keySize && memcmp(key, currNode->key, keySize) == 0)
            {
                free(currNode->value);

                currNode->value = malloc(valueSize);
                currNode->valueSize = valueSize;
                
                memcpy(currNode->value, value, valueSize);
                return currNode->value;
            }

            prevNode = currNode;
            currNode = currNode->next;
        }

        HashTableNode* newNode = malloc(sizeof(HashTableNode));
        newNode->key = malloc(keySize);
        newNode->keySize = keySize;
        newNode->value = malloc(valueSize);
        newNode->valueSize = valueSize;
        newNode->next = NULL;

        if (prevNode)
        {
            prevNode->next = newNode;
        }

        memcpy(newNode->key, key, keySize);
        memcpy(newNode->value, value, valueSize);
        return newNode->value;
    }
    else
    {
        entry = malloc(sizeof(HashTableNode));
        entry->key = malloc(keySize);
        entry->keySize = keySize;
        entry->value = malloc(valueSize);
        entry->valueSize = valueSize;
        entry->next = NULL;

        table->count++;
        table->entries[entryIndex] = entry;

        memcpy(entry->key, key, keySize);
        memcpy(entry->value, value, valueSize);
        return entry->value;
    }
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
    iter->key = NULL;
    iter->keySize = 0;

    iter->value = NULL;
    iter->valueSize = 0;

    iter->internal.table = table;
    iter->internal.entry = NULL;
    iter->internal.index = -1;
    return iter;
}

void htIterFree(HashTableIter* iter)
{
    free(iter);
}

int htIterNext(HashTableIter* iter)
{
    HashTable* table = iter->internal.table;

    int index = iter->internal.index;
    if (iter->internal.entry == NULL || iter->internal.entry->next == NULL)
    {
        index++;
        while (table->entries[index] == NULL && index < table->size)
        {
            index++;
        }

        if (index >= table->size)
        {
            iter->internal.entry = NULL;
            iter->internal.index = table->size;

            iter->key = NULL;
            iter->keySize = 0;

            iter->value = NULL;
            iter->valueSize = 0;

            return 0;
        }
        else
        {
            iter->internal.entry = table->entries[index];
            iter->internal.index = index;

            iter->key = iter->internal.entry->key;
            iter->value = iter->internal.entry->value;
            iter->keySize = iter->internal.entry->keySize;
            iter->valueSize = iter->internal.entry->valueSize;

            return 1;
        }
    }
    else
    {
        iter->internal.entry = iter->internal.entry->next;
        iter->key = iter->internal.entry->key;
        iter->value = iter->internal.entry->value;
        iter->keySize = iter->internal.entry->keySize;
        iter->valueSize = iter->internal.entry->valueSize;
        return 1;
    }
}

void* htIterGetKey(HashTableIter* iter)
{
    return iter->key;
}

void* htIterGetValue(HashTableIter* iter)
{
    return iter->value;
}
