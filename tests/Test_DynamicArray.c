#include <stdio.h>
#include <string.h>

#include "../src/HashTable_DynamicArray.h"

int main(void)
{
    HashTable* testTable = htNew(8, NULL);

    printf("Start insert values to HashTable\n");
    dictInsert(testTable, "Perl", "Language");
    dictInsert(testTable, "GNU", "System");
    dictInsert(testTable, "Java", "Verbose");
    dictInsert(testTable, "Firefox", "Web Browser");

    printf("Iteration values of HashTable\n");
    HashTableIter iter;
    htIteration(testTable, &iter);
    while (htNext(&iter))
    {
        printf("%s => %s\n", (char*)iter.key, (char*)iter.value);
    }

    htFree(testTable);
    return 0;
}