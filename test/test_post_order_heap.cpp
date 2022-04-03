//
// Created by nicolaj on 3/27/22.
//
#include <chrono>
#include <queue>
#include "gtest/gtest.h"
#include "../src/postorder_heap.cpp"

TEST(PostOrderHeap, Print) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 3);
    for (int index = 0; index < 100; index++)
        heap.push(rand());

    for (int index = 0; index < 100; index++) {
        std::cout << std::to_string(heap.top()) << '\n';
        heap.pop();
    }
}

TEST(PostOrderHeap, TestPushIncreasesSize) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 2);
    for (int index = 0; index < 100; index++) {
        EXPECT_EQ(heap.size(), index);
        heap.push(rand());
        EXPECT_EQ(heap.size(), index + 1);
    }
}

TEST(PostOrderHeap, TestPopDecreasesSize) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 2);
    for (int index = 0; index < 100; index++) {
        heap.push(rand());
    }
    for (int index = 100; index > 0; index--) {
        EXPECT_EQ(heap.size(), index);
        heap.pop();
        EXPECT_EQ(heap.size(), index - 1);
    }
}

TEST(PostOrderHeapTest, TestTopDoesntDecreaseSize) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 2);
    for (int index = 0; index < 100; index++) {
        heap.push(rand());
    }
    for (int index = 100; index > 0; index--) {
        EXPECT_EQ(heap.size(), index);
        int top = heap.top();
        EXPECT_EQ(heap.size(), index);
        heap.pop();
    }
}

TEST(PostOrderHeap, TestEmptyIsTrueIfEmpty) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 2);
    EXPECT_TRUE(heap.empty());
    for (int index = 0; index < 100; index++) {
        heap.push(rand());
        EXPECT_FALSE(heap.empty());
    }
    EXPECT_FALSE(heap.empty());
    for (int index = 100; index > 0; index--) {
        EXPECT_FALSE(heap.empty());
        heap.pop();
    }
    EXPECT_TRUE(heap.empty());
}

TEST(PostOrderHeapTest, TestSequentialTopExtractionsReturnDecreasingPriority) {
    auto comparator = [](int left, int right) {
        return left < right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 2);
    for (int index = 0; index < 1000; index++) {
        heap.push(rand());
    }
    int last = std::numeric_limits<int>::min();
    for (int index = 1000; index > 0; index--) {
        int top = heap.top();
        heap.pop();
        EXPECT_TRUE(top >= last);
        last = top;
    }
}
TEST(PostOrderHeapTest, TestSequentialTopExtractionsReturnDecreasingPriority2) {
    auto comparator = [](int left, int right) {
        return left > right;
    };
    postorder_heap<int, std::vector<int>, decltype(comparator)> heap(comparator, 3);
    for (int index = 0; index < 1000000; index++) {
        heap.push(rand());
    }
    int last = std::numeric_limits<int>::max();
    for (int index = 1000000; index > 0; index--) {
        int top = heap.top();
        heap.pop();
        EXPECT_TRUE(top <= last);
        last = top;
    }
}