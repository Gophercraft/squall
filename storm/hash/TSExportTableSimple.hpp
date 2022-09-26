#ifndef STORM_HASH_TS_EXPORT_TABLE_SIMPLE_HPP
#define STORM_HASH_TS_EXPORT_TABLE_SIMPLE_HPP

#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSHashTable.hpp"
#include "storm/hash/TSHashTableReuse.hpp"

template <class T, class THandle, size_t U>
class TSExportTableSimple : public TSHashTableReuse<T, HASHKEY_NONE, U> {
    public:
    HASHKEY_NONE m_key;
    uint32_t m_sequence;
    int32_t m_wrapped;

    THandle GenerateUniqueHandle();
    T* Ptr(THandle handle);
    T* New(THandle* handle);
    void Delete(T* ptr);
};

template <class T, class THandle, size_t U>
THandle TSExportTableSimple<T, THandle, U>::GenerateUniqueHandle() {
    uint32_t nextSequence = 0;

    while (true) {
        for (;;) {
            uint32_t nextSequence = m_sequence + 1;
            m_sequence = nextSequence;
            if (nextSequence) {
                break;
            }
            m_wrapped = 1;
        }

        if (m_wrapped == 0 || Ptr(nextSequence) == nullptr) {
            break;
        }
    }

    return THandle(nextSequence);
}

template <class T, class THandle, size_t U>
T* TSExportTableSimple<T, THandle, U>::Ptr(THandle handle) {
    return TSHashTable<T, HASHKEY_NONE>::Ptr(uint32_t(handle), m_key);
}

template <class T, class THandle, size_t U>
T* TSExportTableSimple<T, THandle, U>::New(THandle* handle) {
    THandle h = GenerateUniqueHandle();
    if (handle != nullptr) {
        *handle = h;
    }
    return TSHashTable<T, HASHKEY_NONE>::New(uint32_t(h), m_key, 0, 0);
}

template <class T, class THandle, size_t U>
void TSExportTableSimple<T, THandle, U>::Delete(T* ptr) {
    TSHashTable<T, HASHKEY_NONE>::Delete(ptr);
}

#endif
