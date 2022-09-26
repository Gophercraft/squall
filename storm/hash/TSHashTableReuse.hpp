#ifndef STORM_HASH_TS_HASH_TABLE_REUSE_HPP
#define STORM_HASH_TS_HASH_TABLE_REUSE_HPP

#include "storm/List.hpp"
#include "storm/array/TSGrowableArray.hpp"
#include "storm/hash/TSHashTable.hpp"

template <class T, class TKey>
class TSHashObjectChunk {
    public:
    TSGrowableArray<T> m_array;
    TSLink<TSHashObjectChunk<T, TKey>> m_link;
};

// TODO: find out what U represents
template <class T, class TKey, size_t U>
class TSHashTableReuse : public TSHashTable<T, TKey> {
    public:
    using ChunkType = TSHashObjectChunk<T, TKey>;

    public:
    STORM_EXPLICIT_LIST(T, m_linktoslot) m_reuseList;
    uint32_t m_chunkSize;
    STORM_EXPLICIT_LIST(ChunkType, m_link) m_chunkList;

    TSHashTableReuse();
    T* InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags);
    void InternalDelete(T* ptr);
    void Destructor();
    void Destroy();
    ~TSHashTableReuse();
};

template <class T, class TKey, size_t U>
TSHashTableReuse<T, TKey, U>::TSHashTableReuse() {
    this->m_chunkSize = 16;
}

template <class T, class TKey, size_t U>
T* TSHashTableReuse<T, TKey, U>::InternalNew(STORM_EXPLICIT_LIST(T, m_linktoslot)* listptr, size_t extrabytes, uint32_t flags) {
    STORM_ASSERT(!extrabytes);
    TSHashObjectChunk<T, TKey>* head = nullptr;

    T* node = this->m_reuseList.Head();

    if (node == nullptr) {
        while (true) {
            head = m_chunkList.Head();

            if (head != nullptr) {
                if (head->m_array.Reserved()) {
                    break;
                }
            }

            TSHashObjectChunk<T, TKey>* newNode = m_chunkList.NewNode(1, 0, 0);
            newNode->m_array.ReserveSpace(m_chunkSize);
            m_chunkSize *= 2;
        }

        node = head->m_array.New();
    }

    listptr->LinkNode(node, 1, nullptr);
    return node;
}

template <class T, class TKey, size_t U>
void TSHashTableReuse<T, TKey, U>::InternalDelete(T* ptr) {
    this->m_fulllist.UnlinkNode(ptr);
    this->m_reuseList.LinkNode(ptr, 1, nullptr);
}

template <class T, class TKey, size_t U>
void TSHashTableReuse<T, TKey, U>::Destructor() {
    this->m_chunkList.Clear();
    this->m_reuseList.Clear();
    this->m_chunkSize = 16;
}

template <class T, class TKey, size_t U>
void TSHashTableReuse<T, TKey, U>::Destroy() {
    // this->Clear();
    this->Destructor();
}

template <class T, class TKey, size_t U>
TSHashTableReuse<T, TKey, U>::~TSHashTableReuse() {
    this->Destructor();
}

#endif
