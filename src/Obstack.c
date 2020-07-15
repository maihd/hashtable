#include "Obstack.h"

#include <stdlib.h>
#include <string.h>

struct ObHeader
{
    struct ObHeader* next;
};

Obstack* obNew(int objectSize, int objectCount)
{
    int bufferSize = (sizeof(struct ObHeader) + objectSize) * objectCount;
    Obstack* stack = malloc(sizeof(Obstack) + bufferSize);
    
    stack->objectSize   = objectSize;
    stack->objectCount  = objectCount;
    stack->head         = (char*)stack + sizeof(Obstack);
    stack->next         = NULL;

    struct ObHeader* head = stack->head;
    for (int i = 0; i < objectCount - 1; i++)
    {
        struct ObHeader* next = (struct ObHeader*)((char*)head + sizeof(struct ObHeader) + objectSize);
        head->next = next;
        head = next;
    }
    head->next = NULL;

    return stack;
}

void obFree(Obstack* stack)
{
    if (stack)
    {
        obFree(stack->next);
        free(stack);
    }
}

void* obAcquire(Obstack* stack)
{
    if (!stack->head)
    {
        return NULL;
    }
    else
    {
        struct ObHeader* head = stack->head;
        stack->head = head->next;

        return head + 1;
    }
}

void obRelease(Obstack* stack, void* object)
{
    struct ObHeader* head = (struct ObHeader*)object - 1;
    head->next = stack->head;
    stack->head = head;
}
