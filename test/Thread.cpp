#include "storm/Thread.hpp"
#include "test/Test.hpp"

TEST_CASE("SGetCurrentThreadId", "[thread]") {
    SECTION("returns thread id") {
        uintptr_t threadId = SGetCurrentThreadId();
        CHECK(threadId > 0);
    }
}

struct testStruct {
    uint32_t num;
    char * text;
};

uint32_t testThread(void* parameter) {
    testStruct * ts = reinterpret_cast<testStruct*>(parameter);
    CHECK(ts->num == 123);
    CHECK(SStrCmp(ts->text, "one two three", -1) == 0);
    return 0;
}

TEST_CASE("SThread::Create", "[thread]") {
    SECTION("creates a thread process") {
        SThread sync;
        auto ts = new testStruct;
        ts->num = 123;
        ts->text = "one two three";
        SThread::Create(testThread, ts, sync, "test thread");
        sync.Wait(-1);
    }
}

TEST_CASE("CSRWLock", "[thread]") {
    SECTION("Constructs") {
        CSRWLock lock;
    }

    SECTION("Locks and unlocks in one thread (fromwriting:0)") {
        int32_t fromwriting = 0;
        CSRWLock lock;
        lock.Enter(fromwriting);
        int i;
        for (i=0;i<10;i++);
        lock.Leave(fromwriting);
    }

    SECTION("Locks and unlocks in one thread (fromwriting:1)") {
        int32_t fromwriting = 1;
        CSRWLock lock;
        lock.Enter(fromwriting);
        int i;
        for (i=0;i<10;i++);
        lock.Leave(fromwriting);
    }
}
