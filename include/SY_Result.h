#ifndef SY_RESULT_H
#define SY_RESULT_H

#include "SY_Common.h"

typedef struct SY_Result
{
  u32 domain;
  u32 code;
} SY_Result;

#define SY_RESULT_TABLE                                                       \
  X (SY_RESULT_PLACEHOLDER)                                                   \
  X (SY_RESULT_ALLOCATOR_ALLOCATION_FAILURE)                                  \
  X (SY_RESULT_MAX)

enum
{
#define X(NAME) NAME,
  SY_RESULT_TABLE
#undef X
};

#define SY_DOMAIN_TABLE                                                       \
  X (SY_DOMAIN_SUCCESS)                                                       \
  X (SY_DOMAIN_GENERAL)                                                       \
  X (SY_DOMAIN_VULKAN)                                                        \
  X (SY_DOMAIN_SDL)                                                           \
  X (SY_DOMAIN_MAX)

enum
{
#define X(NAME) NAME,
  SY_DOMAIN_TABLE
#undef X
};

typedef struct SY_ResultString
{
  const char *const pDomain;
  const char *const pCode;
} SY_ResultString;

const char *SY_Result_GetDomainString (SY_Result result);
const char *SY_Result_GetCodeString (SY_Result result);
const SY_ResultString SY_Result_GetStrings (SY_Result);

#endif
