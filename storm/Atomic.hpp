#ifndef STORM_ATOMIC_HPP
#define STORM_ATOMIC_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>

typedef long ATOMIC32;
typedef LONG64 ATOMIC64;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
typedef int32_t ATOMIC32;
typedef int64_t ATOMIC64;
#endif

ATOMIC32 SInterlockedDecrement(ATOMIC32* ptr);

ATOMIC32 SInterlockedIncrement(ATOMIC32* ptr);

ATOMIC32 SInterlockedCompareExchange(ATOMIC32* destPtr, const ATOMIC32& exchange, const ATOMIC32& comperand);

ATOMIC64 SInterlockedCompareExchange(ATOMIC64* destPtr, const ATOMIC64& exchange, const ATOMIC64& comperand);

void* SInterlockedCompareExchangePointer(void** destPtr, void* exchange, void* comperand);

#endif
