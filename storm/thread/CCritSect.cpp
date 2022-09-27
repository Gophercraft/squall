#include "storm/thread/CCritSect.hpp"

CCritSect::CCritSect() {
#if defined(WHOA_SYSTEM_WIN)
    InitializeCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_init(&this->m_mutex, nullptr);
#endif
}

void CCritSect::Enter(int32_t forwriting) {
#if defined(WHOA_SYSTEM_WIN)
    EnterCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_lock(&this->m_mutex);
#endif
}

void CCritSect::Leave(int32_t forwriting) {
#if defined(WHOA_SYSTEM_WIN)
    LeaveCriticalSection(&this->m_critsect);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_unlock(&this->m_mutex);
#endif
}
