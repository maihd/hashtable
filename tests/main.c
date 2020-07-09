#include <stdio.h>
#include <string.h>

#include "HashTable.h"

int main(void)
{
    // float a = 10.0f;
    // float b = 12.3f;
    // printf("a=%f, b=%f\n", a, b);

    // a = a + b;
    // b = a - b;
    // a = a - b;
    // printf("a=%f, b=%f\n", a, b);

    int a = 10;
    int b = 12;
    printf("a=%d, b=%d\n", a, b);

    a = a + b;
    b = a - b;
    a = a - b;
    //a ^= b ^= a ^= b;
    printf("a=%d, b=%d\n", a, b);
    return 0;
}