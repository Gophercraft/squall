#ifndef STORM_THREAD_S__THREAD_HPP
#define STORM_THREAD_S__THREAD_HPP

#include "storm/Memory.hpp"
#include "storm/String.hpp"
#include "storm/thread/CInitCritSect.hpp"
#include "storm/thread/SThread.hpp"
#include <cstdint>
#include <cstring>

typedef SThread SyncObjectData;

struct SThreadParmBlock {
    uint32_t (*threadProc)(void*);
    void* threadParam;
    uintptr_t threadID;
    SyncObjectData* syncObject;
};

class S_Thread {
    public:
    // Types
    struct SThreadTrack {
        int32_t suspended;
        int32_t live;
        uintptr_t threadId;
        char name[16];
    };

    // Static variables
    static int32_t s_numthreads;
    static int32_t s_maxthreads;
    static uint32_t s_threadID;
    static SThreadTrack s_threads[1024];
    static CInitCritSect s_threadCrit;

    // Static functions
    static uint32_t s_SLaunchThread(void* threadParam);
};

#endif
