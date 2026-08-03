#include "SY_Vector.h"

SY_Result
SY_Vector_ConstCharPtr_Initialize(SY_Vector_ConstCharPtr* pObject,
                                  size_t capacity,
                                  const SY_Allocator* const pAllocator)
{
        SY_Result result = { .domain = SY_DOMAIN_SUCCESS, .code = 0 };
        pObject->values = SY_TALLOCATE(ConstCharPtr, capacity);
        if (!pObject->values) {
                result.domain = SY_DOMAIN_GENERAL;
                result.code = SY_RESULT_ALLOCATOR_ALLOCATION_FAILURE;
                goto exit;
        }
        pObject->capacity = capacity;
        pObject->size = 0;
exit:
        return result;
}

void SY_Vector_ConstCharPtr_PushbackUnsafe(SY_Vector_ConstCharPtr* pObject, ConstCharPtr value)
{
        pObject->values[pObject->size] = value;
        pObject->size += 1;
}

void
SY_Vector_ConstCharPtr_Release(SY_Vector_ConstCharPtr* pObject,
                               const SY_Allocator* const pAllocator)
{
        SY_DEALLOCATE(&pObject->values[0]);
}
