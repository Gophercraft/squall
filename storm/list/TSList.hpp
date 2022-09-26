#ifndef STORM_LIST_TS_LIST_HPP
#define STORM_LIST_TS_LIST_HPP

#include "storm/Error.hpp"
#include "storm/Memory.hpp"
#include "storm/list/TSGetLink.hpp"
#include "storm/list/TSLink.hpp"
#include <cstddef>
#include <cstdint>
#include <typeinfo>

#define STORM_LIST(T) TSList<T, TSGetLink<T>>

template <class T, class TGetLink>
class TSList {
    public:
    // Member variables
    ptrdiff_t m_linkoffset = 0;
    TSLink<T> m_terminator;

    // Member functions
    TSList();
    ~TSList();
    void ChangeLinkOffset(ptrdiff_t linkoffset);
    void Clear();
    void DeleteAll(void);
    T* DeleteNode(T* ptr);
    T* Head(void);
    void InitializeTerminator(void);
    bool IsLinked(T* ptr);
    TSLink<T>* Link(const T* ptr);
    void LinkNode(T* ptr, uint32_t linktype, T* existingptr);
    void LinkToHead(T* ptr);
    void LinkToTail(T* ptr);
    const char* MemFileName() const;
    int32_t MemLineNo() const;
    T* NewNode(uint32_t location, size_t extrabytes, uint32_t flags);
    T* Next(const T* ptr);
    T* RawNext(const T* ptr);
    void SetLinkOffset(ptrdiff_t linkoffset);
    T* Tail(void);
    void UnlinkAll(void);
    void UnlinkNode(T* node);
    void Combine(TSList<T, TGetLink>* list, uint32_t linktype, T* existingptr);
};

template <class T, class TGetLink>
TSList<T, TGetLink>::TSList() {
    this->InitializeTerminator();
}

template <class T, class TGetLink>
TSList<T, TGetLink>::~TSList() {
    this->UnlinkAll();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::ChangeLinkOffset(ptrdiff_t linkoffset) {
    if (linkoffset != this->m_linkoffset) {
        this->UnlinkAll();
        this->SetLinkOffset(linkoffset);
    }
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::DeleteAll() {
    T* node;

    while ((node = this->Head())) {
        this->DeleteNode(node);
    }
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::DeleteNode(T* ptr) {
    T* next = this->Next(ptr);

    ptr->~T();
    SMemFree(ptr, __FILE__, __LINE__, 0);

    return next;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Head() {
    return this->m_terminator.Next();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::InitializeTerminator() {
    this->m_terminator.m_prevlink = &this->m_terminator;

    // Set sentinel node (indicates list end)
    this->m_terminator.m_next = reinterpret_cast<T*>(~reinterpret_cast<uintptr_t>(&this->m_terminator));
}

template <class T, class TGetLink>
bool TSList<T, TGetLink>::IsLinked(T* ptr) {
    return TGetLink::Link(ptr, this->m_linkoffset)->IsLinked();
}

template <class T, class TGetLink>
TSLink<T>* TSList<T, TGetLink>::Link(const T* ptr) {
    if (ptr) {
        return TGetLink::Link(ptr, this->m_linkoffset);
    } else {
        return &this->m_terminator;
    }
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkNode(T* ptr, uint32_t linktype, T* existingptr) {
    TSLink<T>* v5 = this->Link(ptr);

    if (v5->m_prevlink) {
        v5->Unlink();
    }

    TSLink<T>* v7;

    if (existingptr) {
        v7 = this->Link(existingptr);
    } else {
        v7 = &this->m_terminator;
    }

    TSLink<T>* v8;

    switch (linktype) {
    case 1:
        // After existingptr
        v5->m_prevlink = v7;
        v5->m_next = v7->m_next;
        v7->NextLink(this->m_linkoffset)->m_prevlink = v5;
        v7->m_next = ptr;

        break;

    case 2:
        // Before existingptr
        v8 = v7->m_prevlink;
        v5->m_prevlink = v7->m_prevlink;
        v5->m_next = v8->m_next;
        v8->m_next = ptr;
        v7->m_prevlink = v5;

        break;

    default:
        STORM_ERROR_FMT("Invalid case: %s=%u", "linktype", linktype);
        break;
    }
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkToHead(T* ptr) {
    this->LinkNode(ptr, 1, nullptr);
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::LinkToTail(T* ptr) {
    this->LinkNode(ptr, 2, nullptr);
}

template <class T, class TGetLink>
const char* TSList<T, TGetLink>::MemFileName() const {
    return typeid(T).name();
}

template <class T, class TGetLink>
int32_t TSList<T, TGetLink>::MemLineNo() const {
    return -2;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::NewNode(uint32_t location, size_t extrabytes, uint32_t flags) {
    void* m = SMemAlloc(sizeof(T) + extrabytes, this->MemFileName(), this->MemLineNo(), flags | 0x8);

    T* node;

    if (m) {
        node = new (m) T();
    } else {
        node = nullptr;
    }

    if (location) {
        this->LinkNode(node, location, nullptr);
    }

    return node;
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Next(const T* ptr) {
    return this->Link(ptr)->Next();
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::RawNext(const T* ptr) {
    TSLink<T>* link = this->Link(ptr);
    return link->RawNext();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::SetLinkOffset(ptrdiff_t linkoffset) {
    this->m_linkoffset = linkoffset;
    this->InitializeTerminator();
}

template <class T, class TGetLink>
T* TSList<T, TGetLink>::Tail() {
    return this->m_terminator.Prev();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::UnlinkAll() {
    T* node;

    while ((node = this->Head())) {
        this->UnlinkNode(node);
    }
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::Clear() {
    this->DeleteAll();
}

template <class T, class TGetLink>
void TSList<T, TGetLink>::UnlinkNode(T* node) {
    TSLink<T>* link = this->Link(node);
    link->Unlink();
}

// ?Combine@?$TSList@UEvtMessage@@V?$TSGetExplicitLink@UEvtMessage@@@@@@QAEXPAV1@KPAUEvtMessage@@@Z
template <class T, class TGetLink>
void TSList<T, TGetLink>::Combine(TSList<T, TGetLink>* list, uint32_t linktype, T* existingptr) {
    STORM_ASSERT(list);
    STORM_ASSERT(list != this);
    STORM_ASSERT(list->m_linkoffset == m_linkoffset);

    // Check if this and list are already combined
    if (list->m_terminator.m_prevlink != &this->m_terminator) {
        // Not combined, combine now
        TSLink<T>* link = this->Link(existingptr);
        TSLink<T>* prev;
        T* next;

        switch (linktype) {
        // After existingptr
        case 1:
            next = link->m_next;
            link->NextLink(this->m_linkoffset)->m_prevlink = list->m_terminator.m_prevlink;
            link->m_next = list->m_terminator.m_next;
            list->m_terminator.NextLink(list->m_linkoffset)->m_prevlink = link;
            list->m_terminator.m_prevlink->m_next = next;
            list->InitializeTerminator();
            break;
        case 2:
            // Before existingptr
            prev = link->m_prevlink;
            next = prev->m_next;
            link->m_prevlink->m_next = list->Head();
            link->m_prevlink = list->m_terminator.m_prevlink;
            list->m_terminator.NextLink(list->m_linkoffset)->m_prevlink = link->m_prevlink;
            list->InitializeTerminator();
            break;
        default:
            STORM_ERROR_FMT("Invalid case: %s=%u", "linktype", linktype);
            break;
        }
    }
}

#endif
