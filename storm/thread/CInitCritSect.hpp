#ifndef STORM_THREAD_C_CRIT_SECT_HPP
#define STORM_THREAD_C_INIT_CRIT_SECT_HPP

#include "storm/thread/CCritSect.hpp"
#include <cstdint>

class CInitCritSect {
    public:
    int32_t m_spinLock;
    CCritSect* m_critsect;
    char m_critsectData[24];

    int Enter();
    void Leave();
};

#endif
