#include "SY_Result.h"

bool
SY_HasError(SY_Result error)
{
        return error.domain != SY_DOMAIN_SUCCESS ? true : false;
}
