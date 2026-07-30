#ifndef SY_ALLOCATOR_H
#define SY_ALLOCATOR_H

#include "SY_Common.h"
#include <stddef.h>

typedef void* (*PFN_syAllocate)(void* pUserData,
                                size_t allocationSize,
                                size_t alignment);
typedef void (*PFN_syDeallocate)(void* pUserData, void* pMemory);

typedef struct SY_Allocator
{
        PFN_syAllocate pfnAllocate;
        PFN_syDeallocate pfnDeallocate;
        void* pUserData;
} SY_Allocator;

#define SY_ALIGNOF(TYPE) alignof(TYPE)

#define SY_ALLOCATE_(PTR, SIZE, ALIGNMENT)                                     \
        ((PTR)->pfnAllocate(((PTR)->pUserData), (SIZE), (ALIGNMENT)))
#define SY_ALLOCATE(SIZE, ALIGNMENT) SY_ALLOCATE_(pAllocator, SIZE, ALIGNMENT)

#define SY_TALLOCATE_(PTR, TYPE, COUNT)                                        \
        SY_ALLOCATE_(PTR, ((sizeof(TYPE) * (COUNT))), SY_ALIGNOF(TYPE))
#define SY_TALLOCATE(TYPE, COUNT) SY_TALLOCATE_(pAllocator, TYPE, COUNT)

#define SY_DEALLOCATE_(PTR, MEMORY)                                            \
        ((PTR)->pfnDeallocate((PTR)->pUserData, (MEMORY)))
#define SY_DEALLOCATE(MEMORY) SY_DEALLOCATE_(pAllocator, MEMORY)

extern const SY_Allocator* SY_DEFAULT_ALLOCATOR_PTR;
extern const SY_Allocator SY_DEFAULT_ALLOCATOR;

#endif
