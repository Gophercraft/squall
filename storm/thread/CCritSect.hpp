#ifndef STORM_THREAD_C_CRIT_SECT_HPP
#define STORM_THREAD_C_CRIT_SECT_HPP

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <pthread.h>
#endif

#include <cstdint>

class CCritSect {
    public:
    // Member variables
#if defined(WHOA_SYSTEM_WIN)
    CRITICAL_SECTION m_critsect;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_t m_mutex;
#endif

    CCritSect();

    // Member functions
    void Enter(int32_t forwriting = 0);
    void Leave(int32_t forwriting = 0);
};

#endif
