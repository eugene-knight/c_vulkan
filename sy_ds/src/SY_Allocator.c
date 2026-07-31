#include "SY_Allocator.h"
#include <stdlib.h>

void *syDefaultAllocate(void *pUserData, size_t allocationSize, size_t alignment) {
        SY_UNUSED(alignment);
        return malloc(allocationSize);
}
void syDefaultDeallocate(void *pUserData, void *pMemory) {
        free(pMemory);
}

const SY_Allocator SY_DEFAULT_ALLOCATOR = {
        .pfnAllocate = syDefaultAllocate,
        .pfnDeallocate = syDefaultDeallocate,
        .pUserData = nullptr,
};
const SY_Allocator *SY_DEFAULT_ALLOCATOR_PTR = &SY_DEFAULT_ALLOCATOR;
