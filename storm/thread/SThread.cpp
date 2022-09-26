#include "storm/thread/SThread.hpp"
#include "storm/Thread.hpp"

int32_t SThread::Create(uint32_t (*threadProc)(void*), void* param, SThread& thread, char* threadName) {
    uintptr_t threadID;
    uintptr_t threadHandle;

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    thread.int0 = 5;
    thread.m_value1 = 0;
    pthread_cond_init(&thread.m_cond, nullptr);
#endif

    threadHandle = SCreateThread(threadProc, param, &threadID, &thread, threadName, 0);

    return threadHandle != 0;
}
