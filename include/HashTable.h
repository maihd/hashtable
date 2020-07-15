#pragma once

typedef struct HashTable     HashTable;
typedef struct HashTableIter HashTableIter;

HashTable*      htNew(int size, int (*hashFn)(void*, int, int));
void            htFree(HashTable* table);

void            htRemove(HashTable* table, void* key, int keySize);
void*           htSearch(HashTable* table, void* key, int keySize);
void*           htInsert(HashTable* talbe, void* key, int keySize, void* value, int valueSize);

int             htHash(void* key, int keySize, int tableSize);

HashTableIter*  htIterNew(HashTable* table);
void            htIterFree(HashTableIter* iter);
int             htIterNext(HashTableIter* iter);
void*           htIterGetKey(HashTableIter* iter);
void*           htIterGetValue(HashTableIter* iter);

#define dictRemove(table, key)                      htRemove(table, key, strlen(key) + 1)
#define dictSearch(table, key)         (const char*)htSearch(table, key, strlen(key) + 1)
#define dictInsert(table, key, value)  (const char*)htInsert(table, key, strlen(key) + 1, value, strlen(value) + 1)
