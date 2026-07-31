#include "SY_Result.h"

static const char *SY_RESULT_STRINGS[] = {
#define X(NAME) #NAME,
  SY_RESULT_TABLE
#undef X
};
static const char *SY_DOMAIN_STRINGS[] = {
#define X(NAME) #NAME,
  SY_DOMAIN_TABLE
#undef X
};

const char *
SY_Result_GetDomainString (SY_Result result)
{
  if (result.domain >= 0 && result.domain < SY_DOMAIN_MAX)
    {
      return SY_DOMAIN_STRINGS[result.domain];
    }
  return "UNKNOWN_DOMAIN";
}
const char *
SY_Result_GetCodeString (SY_Result result)
{
  if (result.code >= 0 && result.code < SY_RESULT_MAX)
    {
      return SY_RESULT_STRINGS[result.code];
    }
  return "UNKNOWN_CODE";
}
const SY_ResultString
SY_Result_GetStrings (SY_Result result)
{
  if ((result.code >= 0 && result.code < SY_RESULT_MAX)
      && (result.domain >= 0 && result.domain < SY_DOMAIN_MAX))
    {
      return (
          const SY_ResultString){ .pDomain = SY_DOMAIN_STRINGS[result.domain],
                                  .pCode = SY_RESULT_STRINGS[result.code] };
    }
  return (const SY_ResultString){ .pDomain = "UNKNOWN_DOMAIN",
                                  .pCode = "UNKNOWN_CODE" };
}

bool
SY_HasError (SY_Result error)
{
  return error.domain != SY_DOMAIN_SUCCESS ? true : false;
}
