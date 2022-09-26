#include "storm/thread/S_Thread.hpp"

int32_t S_Thread::s_numthreads;

int32_t S_Thread::s_maxthreads = 1024;

uint32_t S_Thread::s_threadID;

S_Thread::SThreadTrack S_Thread::s_threads[1024];

CInitCritSect S_Thread::s_threadCrit;

uint32_t S_Thread::s_SLaunchThread(void* threadParam) {
    auto params = static_cast<SThreadParmBlock*>(threadParam);
    auto threadProc = params->threadProc;
    auto threadData = params->threadParam;
    auto threadId = params->threadID;
    auto syncObject = params->syncObject;
    delete params;

    uint32_t threadVal = threadProc(threadData);

    S_Thread::s_threadCrit.Enter();

    int32_t copybytes = sizeof(SThreadTrack) * (s_numthreads - 1);
    S_Thread::SThreadTrack* track = &S_Thread::s_threads[0];
    while (copybytes >= 0) {
        if (track->live && track->threadId == threadId) {
            std::memcpy(track, track + 1, copybytes);
            S_Thread::s_numthreads--;
        }
        track++;
        copybytes -= sizeof(SThreadTrack);
    }

    S_Thread::s_threadCrit.Leave();

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    if (syncObject) {
        pthread_mutex_lock(&syncObject->m_mutex);
        syncObject->m_value1 = 1;
        pthread_mutex_unlock(&syncObject->m_mutex);
        pthread_cond_signal(&syncObject->m_cond);
    }
#endif

    return threadVal;
}
