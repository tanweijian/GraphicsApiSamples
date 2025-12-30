#include "Memory.h"

void* Memory::Malloc(uint64 size, uint32 alignment)
{
    return _aligned_malloc(size, alignment);
}

void* Memory::Realloc(void* original, uint64 size, uint32 alignment)
{
    return _aligned_realloc(original, size, alignment);
}

void Memory::Free(void* original)
{
    _aligned_free(original);
}
