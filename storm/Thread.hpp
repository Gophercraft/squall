#ifndef STORM_THREAD_HPP
#define STORM_THREAD_HPP

#include "storm/Error.hpp"
#include "storm/thread/CCritSect.hpp"
#include "storm/thread/CInitCritSect.hpp"
#include "storm/thread/CSRWLock.hpp"
#include "storm/thread/SCritSect.hpp"
#include "storm/thread/SEvent.hpp"
#include "storm/thread/SSemaphore.hpp"
#include "storm/thread/SSyncObject.hpp"
#include "storm/thread/SThread.hpp"
#include "storm/thread/S_Thread.hpp"
#include <cstdint>

uintptr_t SCreateThread(size_t dwStackSize, uint32_t (*lpStartAddress)(void*), void* lpParameter, uintptr_t* lpThreadID, SThread* thread, char* threadName);
uintptr_t SCreateThread(uint32_t (*lpStartAddress)(void*), void* lpParameter, uintptr_t* lpThreadID, SThread* thread, char* threadName, size_t dwStackSize);
uintptr_t SGetCurrentThreadId();
int32_t SGetCurrentThreadPriority();
void SSetCurrentThreadPriority(int32_t priority);

#endif
