#include "storm/Hash.hpp"
#include "test/Test.hpp"
#include "storm/thread/CCritSect.hpp"

struct TestHashObject : TSHashObject<TestHashObject, HASHKEY_STRI> {
    uint32_t index = 0;
};

TEST_CASE("TSHashTable", "[hash]") {
    SECTION("constructs correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        REQUIRE(hashTable.Head() == nullptr);
    }
}

TEST_CASE("TSHashTable::Clear", "[hash]") {
    SECTION("clears empty hash table correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }

    SECTION("clears hash table with entry correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.New("testKey", 0, 0x0);
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }

    SECTION("clears hash table with multiple entries correctly") {
        TSHashTable<TestHashObject, HASHKEY_STRI> hashTable;
        hashTable.New("testKey1", 0, 0x0);
        hashTable.New("testKey2", 0, 0x0);
        hashTable.Clear();
        REQUIRE(hashTable.Head() == nullptr);
    }
}

TEST_CASE("TSHashTableReuse", "[hash]") {
    SECTION("constructs correctly") {
        TSHashTableReuse<TestHashObject, HASHKEY_STRI, 1> hashTable;
        REQUIRE(hashTable.Head() == nullptr);
    }
}

TEST_CASE("TSExportTableSync", "[hash]") {
    typedef uintptr_t TestLockhandle;
    typedef uintptr_t Testhandle;

    struct TestExportObject : TSHashObject<TestExportObject, HASHKEY_NONE> {
        uint32_t index = 0;
    };

    SECTION("constructs correctly") {
        TSExportTableSync<TestExportObject, Testhandle, TestLockhandle, CCritSect, 1> syncTable;
        REQUIRE(syncTable.Head() == nullptr);
        Testhandle handle;

        TestExportObject * ptr = syncTable.New(&handle);

        printf("%d\n", (uint32_t)handle);
        syncTable.Delete(handle);
    }
}
