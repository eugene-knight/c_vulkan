#ifndef SY_RESULT_H
#define SY_RESULT_H

#include "SY_Common.h"

typedef struct SY_Result
{
        u32 domain;
        u32 code;
} SY_Result;
enum
{
        SY_PLACEHOLDER_RESULT,
        SY_ALLOCATOR_ALLOCATION_FAILURE_GENERAL,
};
enum
{
        SY_DOMAIN_SUCCESS,
        SY_DOMAIN_GENERAL,
        SY_DOMAIN_VULKAN,
        SY_DOMAIN_SDL,
};

#endif
