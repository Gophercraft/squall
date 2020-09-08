#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

#include <cstdint>

#if defined(NDEBUG)
#define STORM_ASSERT(x)         \
    if (!(x)) {                 \
        SErrSetLastError(0x57); \
        return 0;               \
    }
#else
#define STORM_ASSERT(x)                          \
    if (!(x)) {                                  \
        SErrPrepareAppFatal(__FILE__, __LINE__); \
        SErrDisplayAppFatal(#x);                 \
    }
#endif

void SErrDisplayAppFatal(const char* format, ...);

int32_t SErrDisplayError(uint32_t errorcode, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode, uint32_t a7);

void SErrPrepareAppFatal(const char* filename, int32_t linenumber);

void SErrSetLastError(uint32_t errorcode);

#endif
