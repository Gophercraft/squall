#include "storm/thread/SSyncObject.hpp"

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <cerrno>
#include <sys/time.h>
#include <unistd.h>
#endif

SSyncObject::SSyncObject() {
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    pthread_mutex_init(&this->m_mutex, 0);
#endif
}

bool SSyncObject::Valid() {
#if defined(WHOA_SYSTEM_WIN)
    return this->m_opaqueData != nullptr;
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return this->int0 - 1 <= 4;
#endif
}

uint32_t SSyncObject::Wait(uint32_t timeoutMs) {
#if defined(WHOA_SYSTEM_WIN)
    return WaitForSingleObject(this->m_opaqueData, timeoutMs);
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    if (this->int0 == 6) {
        return OSWAIT_FAILED;
    }

    if (timeoutMs != 0xFFFFFFFF) {
        timeval v10;
        timespec v11;

        if (timeoutMs) {
            gettimeofday(&v10, nullptr);

            v11.tv_sec = v10.tv_sec + timeoutMs / 1000;
            uint32_t v7 = 1000 * v10.tv_usec + 1000000 * (timeoutMs % 1000);
            v11.tv_nsec = v7;

            if (v7 > 999999999) {
                v11.tv_sec = v10.tv_sec + timeoutMs / 1000 + 1;
                v11.tv_nsec = v7 - 1000000000;
            }
        } else {
            v11.tv_sec = 0;
            v11.tv_nsec = 0;
        }

        while (true) {
            int32_t v3 = pthread_mutex_trylock(&this->m_mutex);

            if (!v3) {
                break;
            }

            if (v3 != EBUSY) {
                if (v3 != ETIMEDOUT) {
                    return OSWAIT_FAILED;
                } else {
                    return OSWAIT_TIMEOUT;
                }
            }

            gettimeofday(&v10, nullptr);

            if (v10.tv_sec > v11.tv_sec || (v10.tv_sec == v11.tv_sec && 1000 * v10.tv_usec >= v11.tv_nsec)) {
                return OSWAIT_TIMEOUT;
            }

            usleep(0);
        }

        if (this->int0 == 3) {
            return OSWAIT_OBJECT_0;
        }

        int32_t v4;

        while (true) {
            v4 = this->m_value1;

            if (v4) {
                break;
            }

            int32_t v5 = pthread_cond_timedwait(&this->m_cond, &this->m_mutex, &v11);

            if (v5) {
                pthread_mutex_unlock(&this->m_mutex);

                if (v5 == ETIMEDOUT) {
                    return OSWAIT_TIMEOUT;
                } else {
                    return OSWAIT_FAILED;
                }
            }
        }

        if (this->int0 == 2) {
            this->m_value1 = 0;
        } else if (this->int0 == 4) {
            this->m_value1 = v4 - 1;
        }

        pthread_mutex_unlock(&this->m_mutex);

        return OSWAIT_OBJECT_0;
    }

    pthread_mutex_lock(&this->m_mutex);

    if (this->int0 == 3) {
        return OSWAIT_OBJECT_0;
    }

    while (!this->m_value1) {
        pthread_cond_wait(&this->m_cond, &this->m_mutex);
    }

    if (this->int0 == 2) {
        this->m_value1 = 0;
    } else if (this->int0 == 4) {
        this->m_value1 = this->m_value1 - 1;
    }

    pthread_mutex_unlock(&this->m_mutex);

    return OSWAIT_OBJECT_0;
#endif
}
