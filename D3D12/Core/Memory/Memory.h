#pragma once  
#include "Core/Types/BaseTypes.h"

class Memory
{
public:
    static void* Malloc(uint64 size, uint32 alignment = 0);
    static void* Realloc(void* original, uint64 size, uint32 alignment = 0);
    static void Free(void* original);
};