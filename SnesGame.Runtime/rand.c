#include "rand.h"

Uint32 next = 1;

Uint16 snesRand() // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (Uint16)(next >> 16);
}

void snesSRand(Uint32 seed)
{
    next = seed;
}