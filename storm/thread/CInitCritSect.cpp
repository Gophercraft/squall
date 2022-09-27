#include "CInitCritSect.hpp"

int CInitCritSect::Enter() {
    int result = 0;

    if (this->m_critsect == nullptr) {
#if defined(WHOA_SYSTEM_WIN)
        LONG lresult;
        do {
            lresult = InterlockedExchange((LONG*)&this->m_spinLock, (LONG)1);
        } while (lresult != 0);

        if (this->m_critsect == nullptr) {
            this->m_critsect = reinterpret_cast<CCritSect*>(this->m_critsectData);
            if (this->m_critsect) {
                this->m_critsect->CCritSect::CCritSect();
            }
            result = 1;
        }
        this->m_spinLock = 0;
#endif
        // TODO: implement POSIX behavior
    }

    this->m_critsect->Enter();
    return result;
}

void CInitCritSect::Leave() {
    this->m_critsect->Leave();
}
