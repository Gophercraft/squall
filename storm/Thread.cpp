#include "storm/Thread.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <pthread.h>
#endif

uintptr_t SGetCurrentThreadId() {
#if defined(WHOA_SYSTEM_WIN)
    return GetCurrentThreadId();
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return reinterpret_cast<uintptr_t>(pthread_self());
#endif
}

uintptr_t SCreateThread(size_t dwStackSize, uint32_t (*lpStartAddress)(void*), void* lpParameter, uintptr_t* lpThreadID, SThread* sthread, char* threadName) {
    uintptr_t threadHandle;
    uint32_t dwCreationFlags = 0x0;

    if (!threadName) {
        threadName = "";
    }

    S_Thread::s_threadCrit.Enter();

    if (!S_Thread::s_numthreads) {
        auto& mainThread = S_Thread::s_threads[0];
        mainThread.suspended = 0;
        mainThread.live = 1;
        mainThread.threadId = SGetCurrentThreadId();
        SStrCopy(mainThread.name, "main", sizeof(mainThread.name));

        S_Thread::s_numthreads++;
    }

    S_Thread::s_threadCrit.Leave();

    auto threadParam = new SThreadParmBlock;
    threadParam->threadProc = lpStartAddress;
    threadParam->threadParam = lpParameter;
    threadParam->syncObject = sthread;

#if defined(WHOA_SYSTEM_WIN)
    HANDLE hThread = CreateThread(
        0x0,
        dwStackSize,
        (LPTHREAD_START_ROUTINE)S_Thread::s_SLaunchThread,
        threadParam,
        dwCreationFlags,
        (LPDWORD)lpThreadID);

    threadParam->threadID = *lpThreadID;
    threadParam->syncObject->m_opaqueData = hThread;

    threadHandle = (uintptr_t)hThread;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_t pThread;
    pthread_create(&pThread, nullptr, lpStartAddress, threadParam);
    threadHandle = reinterpret_cast<uintptr_t>(pThread);
    threadParam->threadID = threadHandle;
#endif

    S_Thread::s_threadCrit.Enter();

    STORM_ASSERT(S_Thread::s_numthreads < S_Thread::s_maxthreads);

    auto& thread = S_Thread::s_threads[S_Thread::s_numthreads];
    thread.threadId = threadParam->threadID;
    thread.live = 1;
    thread.suspended = false;
    SStrCopy(thread.name, threadName, sizeof(thread.name));

    S_Thread::s_numthreads++;

    S_Thread::s_threadCrit.Leave();

    return threadHandle;
}

uintptr_t SCreateThread(uint32_t (*lpStartAddress)(void*), void* lpParameter, uintptr_t* lpThreadID, SThread* sthread, char* threadName, size_t dwStackSize) {
    return SCreateThread(dwStackSize, lpStartAddress, lpParameter, lpThreadID, sthread, threadName);
}

int32_t SGetCurrentThreadPriority() {
#if defined(WHOA_SYSTEM_WIN)
    HANDLE thread = GetCurrentThread();
    return (int32_t)GetThreadPriority(thread);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    struct sched_param params;
    int policy = 0;
    pthread_t thread = pthread_self();

    pthread_getschedparams(thread, &policy, &params);

    return params.sched_priority;
#endif
}

void SSetCurrentThreadPriority(int32_t priority) {
#if defined(WHOA_SYSTEM_WIN)
    HANDLE thread = GetCurrentThread();
    SetThreadPriority(thread, priority);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_t thread = pthread_self();
    pthread_setschedprio(thread, priority);
#endif
}
