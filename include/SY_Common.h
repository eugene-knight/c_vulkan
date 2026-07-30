#ifndef SY_COMMON_H
#define SY_COMMON_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define SY_UNUSED(x) (void)x
#define SY_ARRAY_SIZE(x) (sizeof(x)/ sizeof((x)[0]))
#define SY_IGNORE

#endif
