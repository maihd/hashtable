#include <stdio.h>
#include <string.h>

#include "../include/HashTable.h"

int main(void)
{
    HashTable* testTable = htNew(8, NULL);

    printf("Start insert values to HashTable\n");
    dictInsert(testTable, "Perl", "Language");
    dictInsert(testTable, "GNU", "System");
    dictInsert(testTable, "Java", "Verbose");
    dictInsert(testTable, "Firefox", "Web Browser");

    printf("Iteration values of HashTable\n");
    HashTableIter* iter = htIterNew(testTable);
    while (htIterNext(iter))
    {
        printf("%s => %s\n", (char*)htIterGetKey(iter), (char*)htIterGetValue(iter));
    }

    htIterFree(iter);
    htFree(testTable);
    return 0;
}