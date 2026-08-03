#ifndef SY_VECTOR_H
#define SY_VECTOR_H

#include "SY_Allocator.h"
#include "SY_Result.h"

#define SY_VECTOR_APPLY_GENERIC(M) \
        M(SY_Vector_ConstCharPtr *, ConstCharPtr)

#define SY_VECTOR_GENERIC_ROW(TYPE, SUFFIX, OPERATION)                         \
  TYPE:                                                                        \
  SY_Vector_##SUFFIX##_##OPERATION,

#define SY_VECTOR_GENERIC(OPERATION)\
        SY_VECTOR_APPLY_GENERIC(\
                SY_VECTOR_EXPAND_##OPERATION\
        )

#define SY_VECTOR_EXPAND_Initialize(TYPE, SUFFIX)     SY_VECTOR_GENERIC_ROW(TYPE, SUFFIX, Initialize)
#define SY_VECTOR_EXPAND_Release(TYPE, SUFFIX)        SY_VECTOR_GENERIC_ROW(TYPE, SUFFIX, Release)
#define SY_VECTOR_EXPAND_PushbackUnsafe(TYPE, SUFFIX) SY_VECTOR_GENERIC_ROW(TYPE, SUFFIX, PushbackUnsafe)

#define SY_Vector_Initialize(VEC, ...)                                         \
  _Generic((VEC), SY_VECTOR_GENERIC(Initialize) default: NULL)((VEC),          \
                                                               __VA_ARGS__)
#define SY_Vector_PushbackUnsafe(VEC, ...)                                     \
  _Generic((VEC),                                                              \
      SY_VECTOR_GENERIC(PushbackUnsafe) default: NULL)((VEC), __VA_ARGS__)
#define SY_Vector_Release(VEC, ...)                                            \
  _Generic((VEC), SY_VECTOR_GENERIC(Release) default: NULL)((VEC), __VA_ARGS__)

typedef const char *ConstCharPtr;
typedef struct SY_Vector_ConstCharPtr {
  ConstCharPtr *values;
  size_t capacity;
  size_t size;
} SY_Vector_ConstCharPtr;

SY_Result
SY_Vector_ConstCharPtr_Initialize(SY_Vector_ConstCharPtr *pObject,
                                  size_t capacity,
                                  const SY_Allocator *const pAllocator);

void SY_Vector_ConstCharPtr_PushbackUnsafe(SY_Vector_ConstCharPtr *pObject,
                                           ConstCharPtr value);
void SY_Vector_ConstCharPtr_Release(SY_Vector_ConstCharPtr *pObject,
                                    const SY_Allocator *const pAllocator);
#endif
