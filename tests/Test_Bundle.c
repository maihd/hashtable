#include "Bundle.h"
#include <stdio.h>

int main(void)
{
    Bundle* bundle = newBundle(8);

    setI32(bundle, "value", 10);
    printf("value=%d\n", getI32(bundle, "value"));

    freeBundle(bundle);
    return 0;
}