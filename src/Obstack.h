#pragma once

typedef struct Obstack
{
    int objectSize;
    int objectCount;

    void*           head;
    struct Obstack* next;
} Obstack;

Obstack*    obNew(int objectSize, int objectCount);
void        obFree(Obstack* stack);

void*       obAcquire(Obstack* stack);
void        obRelease(Obstack* stack, void* object);
