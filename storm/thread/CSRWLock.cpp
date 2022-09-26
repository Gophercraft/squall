#include "storm/thread/CSRWLock.hpp"

void CSRWLock::Enter(int32_t forwriting) {
#if defined(WHOA_SYSTEM_WIN)
    if (forwriting) {
        AcquireSRWLockExclusive(&m_lock);
    } else {
        AcquireSRWLockShared(&m_lock);
    }
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    if (forwriting) {
        pthread_rwlock_wrlock(&this->m_lock);
    } else {
        pthread_rwlock_rdlock(&this->m_lock);
    }
#endif
}

void CSRWLock::Leave(int32_t fromwriting) {
#if defined(WHOA_SYSTEM_WIN)
    if (fromwriting) {
        ReleaseSRWLockExclusive(&m_lock);
    } else {
        ReleaseSRWLockShared(&m_lock);
    }
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_rwlock_unlock(&this->m_lock);
#endif
}
