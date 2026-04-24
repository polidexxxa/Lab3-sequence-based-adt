#include <gtest/gtest.h>
#include <string>
#include <initializer_list>

#include "Deque.hpp"
#include "Exceptions.hpp"

template <typename T>
static Deque<T> MakeDeque(std::initializer_list<T> values) {
    Deque<T> dq;
    for (const auto& v : values) {
        dq.PushBack(v);
    }
    return dq;
}

TEST(DequeTest, DefaultConstructorCreatesEmptyDeque) {
    Deque<int> dq;
    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.GetSize(), 0);
}

TEST(DequeTest, PushFrontAndPeekWorkCorrectly) {
    Deque<int> dq;
    dq.PushFront(10);
    EXPECT_FALSE(dq.IsEmpty());
    EXPECT_EQ(dq.GetSize(), 1);
    EXPECT_EQ(dq.PeekFront(), 10);
    EXPECT_EQ(dq.PeekBack(), 10);

    dq.PushFront(5);
    EXPECT_EQ(dq.GetSize(), 2);
    EXPECT_EQ(dq.PeekFront(), 5);
    EXPECT_EQ(dq.PeekBack(), 10);
}

TEST(DequeTest, PushBackAndPeekWorkCorrectly) {
    Deque<int> dq;
    dq.PushBack(10);
    EXPECT_FALSE(dq.IsEmpty());
    EXPECT_EQ(dq.GetSize(), 1);
    EXPECT_EQ(dq.PeekFront(), 10);
    EXPECT_EQ(dq.PeekBack(), 10);

    dq.PushBack(20);
    EXPECT_EQ(dq.GetSize(), 2);
    EXPECT_EQ(dq.PeekFront(), 10);
    EXPECT_EQ(dq.PeekBack(), 20);
}

TEST(DequeTest, PopFrontRemovesElementsInCorrectOrder) {
    Deque<int> dq = MakeDeque<int>({10, 20, 30});

    EXPECT_EQ(dq.PopFront(), 10);
    EXPECT_EQ(dq.GetSize(), 2);
    EXPECT_EQ(dq.PeekFront(), 20);
    EXPECT_EQ(dq.PeekBack(), 30);

    EXPECT_EQ(dq.PopFront(), 20);
    EXPECT_EQ(dq.PopFront(), 30);
    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, PopBackRemovesElementsInCorrectOrder) {
    Deque<int> dq = MakeDeque<int>({10, 20, 30});

    EXPECT_EQ(dq.PopBack(), 30);
    EXPECT_EQ(dq.GetSize(), 2);
    EXPECT_EQ(dq.PeekFront(), 10);
    EXPECT_EQ(dq.PeekBack(), 20);

    EXPECT_EQ(dq.PopBack(), 20);
    EXPECT_EQ(dq.PopBack(), 10);
    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, PushFrontAndPushBackCanBeMixed) {
    Deque<int> dq;
    dq.PushBack(20);
    dq.PushFront(10);
    dq.PushBack(30);
    dq.PushFront(5);

    EXPECT_EQ(dq.GetSize(), 4);
    EXPECT_EQ(dq.PeekFront(), 5);
    EXPECT_EQ(dq.PeekBack(), 30);

    EXPECT_EQ(dq.PopFront(), 5);
    EXPECT_EQ(dq.PopFront(), 10);
    EXPECT_EQ(dq.PopFront(), 20);
    EXPECT_EQ(dq.PopFront(), 30);
    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, EmptyDequeThrowsOnPopAndPeek) {
    Deque<int> dq;
    EXPECT_THROW(dq.PopFront(), EmptyContainerException);
    EXPECT_THROW(dq.PopBack(), EmptyContainerException);
    EXPECT_THROW(dq.PeekFront(), EmptyContainerException);
    EXPECT_THROW(dq.PeekBack(), EmptyContainerException);
}

TEST(DequeTest, WorksWithStringType) {
    Deque<std::string> dq;
    dq.PushBack("World");
    dq.PushFront("Hello");
    dq.PushBack("!");

    EXPECT_EQ(dq.PopFront(), "Hello");
    EXPECT_EQ(dq.PopFront(), "World");
    EXPECT_EQ(dq.PopFront(), "!");
    EXPECT_TRUE(dq.IsEmpty());
}

TEST(DequeTest, ConcatCreatesCombinedDequeWithoutChangingSources) {
    Deque<int> dq1 = MakeDeque<int>({1, 2});
    Deque<int> dq2 = MakeDeque<int>({3, 4});

    Deque<int>* pResult = dq1.Concat(dq2);

    ASSERT_NE(pResult, nullptr);
    Deque<int>& result = *pResult;

    ASSERT_EQ(result.GetSize(), 4);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
    EXPECT_EQ(result.PopFront(), 4);

    EXPECT_EQ(dq1.GetSize(), 2);
    EXPECT_EQ(dq2.GetSize(), 2);
    EXPECT_EQ(dq1.PeekFront(), 1);
    EXPECT_EQ(dq2.PeekFront(), 3);

    delete pResult;
}

TEST(DequeTest, MergeAppendsElementsFromOtherDeque) {
    Deque<int> dq1 = MakeDeque<int>({1, 2});
    Deque<int> dq2 = MakeDeque<int>({3, 4});

    dq1.Merge(dq2);

    ASSERT_EQ(dq1.GetSize(), 4);
    EXPECT_EQ(dq1.PopFront(), 1);
    EXPECT_EQ(dq1.PopFront(), 2);
    EXPECT_EQ(dq1.PopFront(), 3);
    EXPECT_EQ(dq1.PopFront(), 4);

    EXPECT_TRUE(dq2.IsEmpty());
}

TEST(DequeTest, GetSubcontainerReturnsCorrectRange) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3, 4, 5});

    Deque<int>* pSub = dq.GetSubcontainer(1, 3);
    ASSERT_NE(pSub, nullptr);
    Deque<int>& sub = *pSub;

    ASSERT_EQ(sub.GetSize(), 3);
    EXPECT_EQ(sub.PopFront(), 2);
    EXPECT_EQ(sub.PopFront(), 3);
    EXPECT_EQ(sub.PopFront(), 4);

    delete pSub;
}

TEST(DequeTest, GetSubdequeThrowsForInvalidIndices) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3});

    EXPECT_THROW(dq.GetSubcontainer(-1, 1), IndexOutOfRangeException);
    EXPECT_THROW(dq.GetSubcontainer(0, 5), IndexOutOfRangeException);
    EXPECT_THROW(dq.GetSubcontainer(2, 1), IndexOutOfRangeException);
}

TEST(DequeTest, CopyConstructorCreatesIndependentCopy) {
    Deque<int> original = MakeDeque<int>({1, 2, 3});
    Deque<int> copy(original);

    ASSERT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.PopFront(), 1);
    EXPECT_EQ(copy.PopFront(), 2);
    EXPECT_EQ(copy.PopFront(), 3);

    EXPECT_EQ(original.GetSize(), 3);
    EXPECT_EQ(original.PeekFront(), 1);
    EXPECT_EQ(original.PeekBack(), 3);
}

TEST(DequeTest, AssignmentOperatorCreatesIndependentCopy) {
    Deque<int> original = MakeDeque<int>({1, 2});
    Deque<int> copy;
    copy = original;

    ASSERT_EQ(copy.GetSize(), 2);
    EXPECT_EQ(copy.PopFront(), 1);
    EXPECT_EQ(copy.PopFront(), 2);

    EXPECT_EQ(original.GetSize(), 2);
    EXPECT_EQ(original.PeekFront(), 1);
}

TEST(DequeTest, ClearRemovesAllElementsAndDequeCanBeReused) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3});

    dq.Clear();

    EXPECT_TRUE(dq.IsEmpty());
    EXPECT_EQ(dq.GetSize(), 0);

    dq.PushBack(10);
    dq.PushFront(5);

    EXPECT_FALSE(dq.IsEmpty());
    EXPECT_EQ(dq.GetSize(), 2);
    EXPECT_EQ(dq.PeekFront(), 5);
    EXPECT_EQ(dq.PeekBack(), 10);
}

TEST(DequeTest, MergeSortSortsDequeInAscendingOrder) {
    Deque<int> dq = MakeDeque<int>({5, 1, 4, 2, 3});

    dq.MergeSort();

    EXPECT_EQ(dq.GetSize(), 5);
    EXPECT_EQ(dq.PopFront(), 1);
    EXPECT_EQ(dq.PopFront(), 2);
    EXPECT_EQ(dq.PopFront(), 3);
    EXPECT_EQ(dq.PopFront(), 4);
    EXPECT_EQ(dq.PopFront(), 5);
}

TEST(DequeTest, MergeSortKeepsSingleElementDequeUnchanged) {
    Deque<int> dq;
    dq.PushBack(42);

    dq.MergeSort();

    EXPECT_EQ(dq.GetSize(), 1);
    EXPECT_EQ(dq.PeekFront(), 42);
    EXPECT_EQ(dq.PeekBack(), 42);
}

TEST(DequeTest, IsSortedReturnsTrueForStrictlyIncreasingDeque) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3, 4, 5});
    EXPECT_TRUE(dq.IsSorted());
}

TEST(DequeTest, IsSortedReturnsFalseForUnsortedDeque) {
    Deque<int> dq = MakeDeque<int>({1, 3, 2, 4, 5});
    EXPECT_FALSE(dq.IsSorted());
}

TEST(DequeTest, ContainsSubsequenceReturnsTrueForExistingSubsequence) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3, 4, 5});
    Deque<int> sub = MakeDeque<int>({2, 3, 4});

    EXPECT_TRUE(dq.ContainsSubsequence(sub));
}

TEST(DequeTest, ContainsSubsequenceReturnsFalseForMissingSubsequence) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3, 4, 5});
    Deque<int> sub = MakeDeque<int>({2, 4});

    EXPECT_FALSE(dq.ContainsSubsequence(sub));
}

TEST(DequeTest, ContainsSubsequenceReturnsTrueForEmptySubsequence) {
    Deque<int> dq = MakeDeque<int>({1, 2, 3});
    Deque<int> sub;

    EXPECT_TRUE(dq.ContainsSubsequence(sub));
}

TEST(DequeTest, ContainsSubsequenceWorksForStrings) {
    Deque<std::string> dq = MakeDeque<std::string>({"aa", "bb", "cc", "dd"});
    Deque<std::string> sub = MakeDeque<std::string>({"bb", "cc"});

    EXPECT_TRUE(dq.ContainsSubsequence(sub));
}