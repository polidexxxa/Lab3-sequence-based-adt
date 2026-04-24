#include <gtest/gtest.h>
#include <string>
#include <initializer_list>
#include "Queue.hpp"
#include "Exceptions.hpp"

template <typename T>
static Queue<T> MakeQueue(std::initializer_list<T> values) {
    Queue<T> q;
    for (const auto& v : values) {
        q.Enqueue(v);
    }
    return q;
}

TEST(QueueTest, DefaultConstructorCreatesEmptyQueue) {
    Queue<int> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetSize(), 0);
}

TEST(QueueTest, EnqueueAndPeekWorkCorrectly) {
    Queue<int> q;
    q.Enqueue(10);
    EXPECT_FALSE(q.IsEmpty());
    EXPECT_EQ(q.GetSize(), 1);
    EXPECT_EQ(q.Peek(), 10);

    q.Enqueue(20);
    EXPECT_EQ(q.GetSize(), 2);
    EXPECT_EQ(q.Peek(), 10);
}

TEST(QueueTest, DequeuePreservesFifoOrder) {
    Queue<int> q = MakeQueue<int>({10, 20, 30});

    EXPECT_EQ(q.Dequeue(), 10);
    EXPECT_EQ(q.GetSize(), 2);
    EXPECT_EQ(q.Peek(), 20);

    EXPECT_EQ(q.Dequeue(), 20);
    EXPECT_EQ(q.Dequeue(), 30);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(QueueTest, EmptyQueueThrowsOnDequeueAndPeek) {
    Queue<int> q;
    EXPECT_THROW(q.Dequeue(), EmptyContainerException);
    EXPECT_THROW(q.Peek(), EmptyContainerException);
}

TEST(QueueTest, WorksWithStringType) {
    Queue<std::string> q;
    q.Enqueue("Hello");
    q.Enqueue("World");
    q.Enqueue("!");

    EXPECT_EQ(q.Dequeue(), "Hello");
    EXPECT_EQ(q.Dequeue(), "World");
    EXPECT_EQ(q.Dequeue(), "!");
    EXPECT_TRUE(q.IsEmpty());
}

TEST(QueueTest, ConcatCreatesCombinedQueueWithoutChangingSources) {
    Queue<int> q1 = MakeQueue<int>({1, 2});
    Queue<int> q2 = MakeQueue<int>({3, 4});

    Queue<int>* pResult = q1.Concat(q2);

    ASSERT_NE(pResult, nullptr);
    Queue<int>& result = *pResult;

    ASSERT_EQ(result.GetSize(), 4);
    EXPECT_EQ(result.Dequeue(), 1);
    EXPECT_EQ(result.Dequeue(), 2);
    EXPECT_EQ(result.Dequeue(), 3);
    EXPECT_EQ(result.Dequeue(), 4);

    EXPECT_EQ(q1.GetSize(), 2);
    EXPECT_EQ(q2.GetSize(), 2);
    EXPECT_EQ(q1.Peek(), 1);
    EXPECT_EQ(q2.Peek(), 3);

    delete pResult;
}

TEST(QueueTest, MergeAppendsElementsFromOtherQueue) {
    Queue<int> q1 = MakeQueue<int>({1, 2});
    Queue<int> q2 = MakeQueue<int>({3, 4});

    q1.Merge(q2);

    ASSERT_EQ(q1.GetSize(), 4);
    EXPECT_EQ(q1.Dequeue(), 1);
    EXPECT_EQ(q1.Dequeue(), 2);
    EXPECT_EQ(q1.Dequeue(), 3);
    EXPECT_EQ(q1.Dequeue(), 4);
    EXPECT_TRUE(q2.IsEmpty());
}

TEST(QueueTest, GetSubcontainerReturnsCorrectRange) {
    Queue<int> q = MakeQueue<int>({1, 2, 3, 4, 5});

    Queue<int>* pSub = q.GetSubcontainer(1, 3);
    ASSERT_NE(pSub, nullptr);
    Queue<int>& sub = *pSub;

    ASSERT_EQ(sub.GetSize(), 3);
    EXPECT_EQ(sub.Dequeue(), 2);
    EXPECT_EQ(sub.Dequeue(), 3);
    EXPECT_EQ(sub.Dequeue(), 4);

    delete pSub;
}

TEST(QueueTest, SplitSeparatesElementsByPredicate) {
    Queue<int> q = MakeQueue<int>({1, 2, 3, 4, 5});

    auto result = q.Split([](int x) { return x % 2 == 0; });

    ASSERT_EQ(result.first.GetSize(), 2);
    ASSERT_EQ(result.second.GetSize(), 3);

    EXPECT_EQ(result.first.Dequeue(), 2);
    EXPECT_EQ(result.first.Dequeue(), 4);

    EXPECT_EQ(result.second.Dequeue(), 1);
    EXPECT_EQ(result.second.Dequeue(), 3);
    EXPECT_EQ(result.second.Dequeue(), 5);
}

TEST(QueueTest, CopyConstructorCreatesIndependentCopy) {
    Queue<int> original = MakeQueue<int>({1, 2, 3});
    Queue<int> copy(original);

    ASSERT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.Dequeue(), 1);
    EXPECT_EQ(copy.Dequeue(), 2);
    EXPECT_EQ(copy.Dequeue(), 3);

    EXPECT_EQ(original.GetSize(), 3);
    EXPECT_EQ(original.Peek(), 1);
}

TEST(QueueTest, AssignmentOperatorCreatesIndependentCopy) {
    Queue<int> original = MakeQueue<int>({1, 2});
    Queue<int> copy;
    copy = original;

    ASSERT_EQ(copy.GetSize(), 2);
    EXPECT_EQ(copy.Dequeue(), 1);
    EXPECT_EQ(copy.Dequeue(), 2);

    EXPECT_EQ(original.GetSize(), 2);
    EXPECT_EQ(original.Peek(), 1);
}

TEST(QueueTest, ClearRemovesAllElementsAndQueueCanBeReused) {
    Queue<int> q = MakeQueue<int>({1, 2, 3});

    q.Clear();

    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetSize(), 0);

    q.Enqueue(10);
    EXPECT_FALSE(q.IsEmpty());
    EXPECT_EQ(q.GetSize(), 1);
    EXPECT_EQ(q.Peek(), 10);
}

TEST(QueueTest, ContainsSubsequenceReturnsTrueForExistingSubsequence) {
    Queue<int> q = MakeQueue<int>({1, 2, 3, 4, 5});
    Queue<int> sub = MakeQueue<int>({2, 3, 4});

    EXPECT_TRUE(q.ContainsSubsequence(sub));
}

TEST(QueueTest, ContainsSubsequenceReturnsFalseForMissingSubsequence) {
    Queue<int> q = MakeQueue<int>({1, 2, 3, 4, 5});
    Queue<int> sub = MakeQueue<int>({2, 4});

    EXPECT_FALSE(q.ContainsSubsequence(sub));
}

TEST(QueueTest, ContainsSubsequenceReturnsTrueForEmptySubsequence) {
    Queue<int> q = MakeQueue<int>({1, 2, 3});
    Queue<int> sub;

    EXPECT_TRUE(q.ContainsSubsequence(sub));
}

TEST(QueueTest, ContainsSubsequenceWorksForStrings) {
    Queue<std::string> q = MakeQueue<std::string>({"aa", "bb", "cc", "dd"});
    Queue<std::string> sub = MakeQueue<std::string>({"bb", "cc"});

    EXPECT_TRUE(q.ContainsSubsequence(sub));
}