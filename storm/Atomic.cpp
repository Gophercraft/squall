#include "storm/Atomic.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

ATOMIC32 SInterlockedDecrement(ATOMIC32* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedDecrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return __sync_fetch_and_sub(ptr, 1) - 1;
#endif
}

ATOMIC32 SInterlockedIncrement(ATOMIC32* ptr) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedIncrement(ptr);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return __sync_fetch_and_add(ptr, 1) + 1;
#endif
}

ATOMIC32 SInterlockedCompareExchange(ATOMIC32* destPtr, ATOMIC32 const& exchange, ATOMIC32 const& comperand) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedCompareExchange(destPtr, exchange, comperand);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    __sync_val_compare_and_swap(destPtr, comperand, exchange);
#endif
}

ATOMIC64 SInterlockedCompareExchange(ATOMIC64* destPtr, ATOMIC64 const& exchange, ATOMIC64 const& comperand) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedCompareExchange64(destPtr, exchange, comperand);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    __sync_val_compare_and_swap(destPtr, comperand, exchange);
#endif
}

void* SInterlockedCompareExchangePointer(void** destPtr, void* exchange, void* comperand) {
#if defined(WHOA_SYSTEM_WIN)
    return InterlockedCompareExchangePointer(destPtr, exchange, comperand);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    __sync_val_compare_and_swap(destPtr, comperand, exchange);
#endif
}
