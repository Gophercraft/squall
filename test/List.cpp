#include "storm/List.hpp"
#include "test/Test.hpp"

#include "Windows.h"

struct TestListNode : TSLinkedNode<TestListNode> {
    uint32_t index = 0;
};

TEST_CASE("TSList", "[list]") {
    SECTION("constructs correctly") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Head() == nullptr);
    }
}

TEST_CASE("TSList::Head", "[list]") {
    SECTION("returns nullptr for uninitialized list") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Head() == nullptr);
    }
}

TEST_CASE("TSList::LinkToHead", "[list]") {
    SECTION("links node to head correctly") {
        STORM_LIST(TestListNode) list;

        auto node = new TestListNode();
        list.LinkToHead(node);

        REQUIRE(list.Head() == node);

        delete node;
    }

    SECTION("links multiple nodes to head correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToHead(node1);

        auto node2 = new TestListNode();
        list.LinkToHead(node2);

        REQUIRE(list.Head() == node2);
        REQUIRE(list.Tail() == node1);

        delete node1;
        delete node2;
    }

    SECTION("links node to head after linking node to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToTail(node1);

        auto node2 = new TestListNode();
        list.LinkToHead(node2);

        REQUIRE(list.Head() == node2);
        REQUIRE(list.Tail() == node1);

        delete node1;
        delete node2;
    }
}

TEST_CASE("TSList::LinkToTail", "[list]") {
    SECTION("links node to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node = new TestListNode();
        list.LinkToTail(node);

        REQUIRE(list.Tail() == node);

        delete node;
    }

    SECTION("links multiple nodes to tail correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToTail(node1);

        auto node2 = new TestListNode();
        list.LinkToTail(node2);

        REQUIRE(list.Head() == node1);
        REQUIRE(list.Tail() == node2);

        delete node1;
        delete node2;
    }

    SECTION("links node to tail after linking node to head correctly") {
        STORM_LIST(TestListNode) list;

        auto node1 = new TestListNode();
        list.LinkToHead(node1);

        auto node2 = new TestListNode();
        list.LinkToTail(node2);

        REQUIRE(list.Head() == node1);
        REQUIRE(list.Tail() == node2);

        delete node1;
        delete node2;
    }
}

TEST_CASE("TSList::Tail", "[list]") {
    SECTION("returns nullptr for uninitialized list") {
        STORM_LIST(TestListNode) list;
        REQUIRE(list.Tail() == nullptr);
    }
}

TEST_CASE("TSList::Combine", "[list]") {
    SECTION("combines after existing pointer") {
        STORM_LIST(TestListNode) list1;
        STORM_LIST(TestListNode) list2;
        TestListNode* cursor;

        for (uint32_t i = 0; i < 10; i++) {
            TestListNode* node = list1.NewNode(2, 0, 0);
            node->index = i;
        }

        cursor = list1.Head();
        while (cursor != nullptr) {
            cursor = cursor->Next();
        }

        for (uint32_t i = 500; i < 510; i++) {
            TestListNode* node = list2.NewNode(2, 0, 0);
            node->index = i;
        }

        list1.Combine(&list2, 1, nullptr);

        cursor = list1.Head();
        while (cursor != nullptr) {
            printf("%d\n", cursor->index);
            cursor = cursor->Next();
        }
        printf("End\n");

        REQUIRE(list1.Head()->index == 500);
        REQUIRE(list1.Tail()->index == 9);
    }

    SECTION("combines before existing pointer") {
        STORM_LIST(TestListNode) list1;
        STORM_LIST(TestListNode) list2;
        TestListNode* cursor;

        for (uint32_t i = 0; i < 10; i++) {
            TestListNode* node = list1.NewNode(2, 0, 0);
            node->index = i;
        }

        cursor = list1.Head();
        while (cursor != nullptr) {
            cursor = cursor->Next();
        }

        for (uint32_t i = 500; i < 510; i++) {
            TestListNode* node = list2.NewNode(2, 0, 0);
            node->index = i;
        }

        list1.Combine(&list2, 2, nullptr);

        cursor = list1.Head();
        while (cursor != nullptr) {
            printf("%d\n", cursor->index);
            cursor = cursor->Next();
        }
        printf("End\n");

        REQUIRE(list1.Head()->index == 0);
        REQUIRE(list1.Tail()->index == 509);
    }
}
