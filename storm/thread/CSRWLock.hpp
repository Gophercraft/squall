#ifndef STORM_THREAD_CS_RW_LOCK_HPP
#define STORM_THREAD_CS_RW_LOCK_HPP

#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)

#include <windows.h>

#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <pthread.h>
#endif

class CSRWLock {
    public:
    // Member variables
#if defined(WHOA_SYSTEM_WIN)
    SRWLOCK m_lock;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_rwlock_t m_lock;
#endif

    // Member functions
    void Enter(int32_t forwriting);
    void Leave(int32_t fromwriting);
};

#endif
