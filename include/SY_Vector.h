#ifndef SY_VECTOR_H
#define SY_VECTOR_H

#include "SY_Allocator.h"
#include "SY_Result.h"

typedef const char* ConstCharPtr;
typedef struct SY_Vector_ConstCharPtr
{
        ConstCharPtr* values;
        size_t capacity;
        size_t size;
} SY_Vector_ConstCharPtr;

SY_Result
SY_Vector_ConstCharPtr_Initialize(SY_Vector_ConstCharPtr* pObject,
                                  size_t capacity,
                                  const SY_Allocator* const pAllocator);

void
SY_Vector_ConstCharPtr_PushbackUnsafe(SY_Vector_ConstCharPtr* pObject,
                                      ConstCharPtr value);
void
SY_Vector_ConstCharPtr_Release(SY_Vector_ConstCharPtr* pObject,
                               const SY_Allocator* const pAllocator);
#endif
