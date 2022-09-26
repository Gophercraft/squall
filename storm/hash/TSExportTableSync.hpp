#ifndef STORM_HASH_TS_EXPORT_TABLE_SYNC_HPP
#define STORM_HASH_TS_EXPORT_TABLE_SYNC_HPP

#include "storm/hash/TSExportTableSimple.hpp"

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
class TSExportTableSync : public TSExportTableSimple<T, THandle, U> {
    public:
    TSync m_sync;

    int32_t IsForWriting(TLockedHandle lockedhandle);
    T* NewLock(THandle* handle, TLockedHandle* lockedhandle);
    T* Lock(THandle handle, TLockedHandle* lockedhandle, int32_t forwriting);
    void Unlock(TLockedHandle lockedhandle);
    void DeleteUnlock(T* ptr, TLockedHandle lockedhandle);
    void Delete(THandle handle);
    void SyncEnterLock(TLockedHandle* lockedhandle, int32_t forwriting);
    void SyncLeaveLock(TLockedHandle lockedhandle);
};

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
void TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::SyncEnterLock(TLockedHandle* lockedhandle, int32_t forwriting) {
    m_sync.Enter(forwriting);
    *lockedhandle = (TLockedHandle)forwriting;
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
void TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::SyncLeaveLock(TLockedHandle lockedhandle) {
    if (lockedhandle) {
        m_sync.Leave(IsForWriting(lockedhandle));
    }
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
int32_t TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::IsForWriting(TLockedHandle lockedhandle) {
    return lockedhandle == TLockedHandle(1);
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
T* TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::Lock(THandle handle, TLockedHandle* lockedhandle, int32_t forwriting) {
    SyncEnterLock(lockedhandle, forwriting);
    T* ptr = Ptr(handle);
    if (ptr == nullptr) {
        SyncLeaveLock(*lockedhandle);
        *lockedhandle = 0;
    }
    return ptr;
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
void TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::Unlock(TLockedHandle lockedhandle) {
    SyncLeaveLock(lockedhandle);
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
T* TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::NewLock(THandle* handle, TLockedHandle* lockedhandle) {
    SyncEnterLock(lockedhandle, 1);
    return New(handle);
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
void TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::DeleteUnlock(T* ptr, TLockedHandle lockedhandle) {
    TSExportTableSimple<T, THandle, U>::Delete(ptr);
    Unlock(lockedhandle);
}

template <class T, class THandle, class TLockedHandle, class TSync, size_t U>
void TSExportTableSync<T, THandle, TLockedHandle, TSync, U>::Delete(THandle handle) {
    T* ptr = Lock(handle, &handle, 1);
    DeleteUnlock(ptr, handle);
}

#endif
