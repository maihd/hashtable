#include "DynamicArray.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DynamicArray* daNew(int capacity, int elementSize)
{
    assert(capacity >= 0);
    assert(elementSize > 0);

    DynamicArray* array = malloc(sizeof(DynamicArray));
    array->count = 0;
    array->capacity = capacity;
    array->elementSize = elementSize;
    array->elements = capacity > 0 ? malloc(capacity * elementSize) : NULL;

    return array;
}

void daFree(DynamicArray* array)
{
    if (array)
    {
        free(array->elements);
        free(array);
    }
}

void daPush(DynamicArray* array, const void* element)
{
    if (daEnsure(array, array->count + 1))
    {
        memcpy((char*)array->elements + array->count * array->elementSize, element, array->elementSize);
        array->count++;
    }
}

void daGet(DynamicArray* array, int index, void* outElement)
{
    assert(index > -1 && index < array->count);
    
    memcpy(outElement, (char*)array->elements + index * array->elementSize, array->elementSize);
}

void daSet(DynamicArray* array, int index, const void* element)
{
    assert(index > -1);

    if (index >= array->count)
    {
        if (!daEnsure(array, index + 1))
        {
            return false;
        }

        array->count = index + 1;
    }

    memcpy((char*)array->elements + index * array->elementSize, element, array->elementSize);
}

void daPop(DynamicArray* array, void* outElement)
{
    assert(array->count > 0);

    array->count--;
    memcpy(outElement, (char*)array->elements + array->count * array->elementSize, array->elementSize);
}

bool daGrow(DynamicArray* array, int capacity)
{
    if (array->capacity >= capacity)
    {
        return true;
    }

    int oldCapacity = array->capacity;
    int newCapacity = capacity - 1;
    newCapacity = newCapacity | (newCapacity >> 1);
    newCapacity = newCapacity | (newCapacity >> 2);
    newCapacity = newCapacity | (newCapacity >> 4);
    newCapacity = newCapacity | (newCapacity >> 8);
    newCapacity = newCapacity | (newCapacity >> 16);
    newCapacity = newCapacity + 1;
    
    void* newElements = realloc(array->elements, newCapacity * array->elementSize);
    if (newElements) 
    {
        array->capacity = newCapacity;
        array->elements = newElements;
        return true;
    }
    
    return false;
}

void daClear(DynamicArray* array)
{
    array->count = 0;
}

bool daEnsure(DynamicArray* array, int capacity)
{
    if (array->capacity < capacity)
    {
        return daGrow(array, capacity);
    }
    else
    {
        return true;
    }
}
