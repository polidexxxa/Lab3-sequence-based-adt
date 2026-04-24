#include <gtest/gtest.h>

#include "Stack.hpp"
#include "ListSequence.hpp"
#include "Exceptions.hpp"

TEST(StackTest, NewStackIsEmpty) {
    Stack<int> st;
    EXPECT_TRUE(st.IsEmpty());
    EXPECT_EQ(st.GetSize(), 0);
}

TEST(StackTest, PushChangesSizeAndPeek) {
    Stack<int> st;
    st.Push(10);
    st.Push(20);
    st.Push(30);

    EXPECT_FALSE(st.IsEmpty());
    EXPECT_EQ(st.GetSize(), 3);
    EXPECT_EQ(st.Peek(), 30);
}

TEST(StackTest, PopReturnsElementsInLifoOrder) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);

    EXPECT_EQ(st.Pop(), 3);
    EXPECT_EQ(st.Pop(), 2);
    EXPECT_EQ(st.Pop(), 1);
    EXPECT_TRUE(st.IsEmpty());
}

TEST(StackTest, PopFromEmptyThrows) {
    Stack<int> st;
    EXPECT_THROW(st.Pop(), EmptyContainerException);
}

TEST(StackTest, PeekFromEmptyThrows) {
    Stack<int> st;
    EXPECT_THROW(st.Peek(), EmptyContainerException);
}

TEST(StackTest, ClearMakesStackEmpty) {
    Stack<int> st;
    st.Push(5);
    st.Push(6);
    st.Push(7);

    st.Clear();

    EXPECT_TRUE(st.IsEmpty());
    EXPECT_EQ(st.GetSize(), 0);
    EXPECT_THROW(st.Peek(), EmptyContainerException);
}

TEST(StackTest, CopyConstructorCreatesIndependentCopy) {
    Stack<int> original;
    original.Push(1);
    original.Push(2);
    original.Push(3);

    Stack<int> copy(original);

    EXPECT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.Peek(), 3);

    original.Pop();
    original.Push(100);

    EXPECT_EQ(copy.Peek(), 3);
    EXPECT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(original.Peek(), 100);
}

TEST(StackTest, AssignmentCreatesIndependentCopy) {
    Stack<int> a;
    a.Push(10);
    a.Push(20);

    Stack<int> b;
    b.Push(1);
    b.Push(2);
    b.Push(3);

    b = a;

    EXPECT_EQ(b.GetSize(), 2);
    EXPECT_EQ(b.Peek(), 20);

    a.Pop();
    a.Push(99);

    EXPECT_EQ(b.Peek(), 20);
    EXPECT_EQ(a.Peek(), 99);
}

TEST(StackTest, ConstructFromSequencePreservesOrder) {
    int items[] = {1, 2, 3, 4};
    ListSequence<int> seq(items, 4);

    Stack<int> st(seq);

    EXPECT_EQ(st.GetSize(), 4);
    EXPECT_EQ(st.Peek(), 4);

    EXPECT_EQ(st.Pop(), 4);
    EXPECT_EQ(st.Pop(), 3);
    EXPECT_EQ(st.Pop(), 2);
    EXPECT_EQ(st.Pop(), 1);
}

TEST(StackTest, GetSubstackReturnsCorrectPart) {
    Stack<int> st;
    st.Push(10);
    st.Push(20);
    st.Push(30);
    st.Push(40);
    st.Push(50);

    Stack<int>* sub = st.GetSubcontainer(1, 3);

    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->GetSize(), 3);

    EXPECT_EQ(sub->Pop(), 40);
    EXPECT_EQ(sub->Pop(), 30);
    EXPECT_EQ(sub->Pop(), 20);

    delete sub;
}

TEST(StackTest, GetSubcontainerInvalidIndicesThrow) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);

    EXPECT_THROW(st.GetSubcontainer(-1, 1), IndexOutOfRangeException);
    EXPECT_THROW(st.GetSubcontainer(0, 5), IndexOutOfRangeException);
    EXPECT_THROW(st.GetSubcontainer(2, 1), IndexOutOfRangeException);
}

TEST(StackTest, ConcatPreservesStackOrder) {
    Stack<int> a;
    a.Push(1);
    a.Push(2);

    Stack<int> b;
    b.Push(3);
    b.Push(4);

    Stack<int>* c = a.Concat(b);

    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->GetSize(), 4);

    EXPECT_EQ(c->Pop(), 4);
    EXPECT_EQ(c->Pop(), 3);
    EXPECT_EQ(c->Pop(), 2);
    EXPECT_EQ(c->Pop(), 1);

    delete c;
}

TEST(StackTest, MapTransformsElements) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);

    auto* mapped = st.Map<int>([](int x) { return x * 10; });

    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(mapped->GetSize(), 3);

    EXPECT_EQ(mapped->Pop(), 30);
    EXPECT_EQ(mapped->Pop(), 20);
    EXPECT_EQ(mapped->Pop(), 10);

    delete mapped;
}

TEST(StackTest, WhereFiltersElements) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);
    st.Push(4);
    st.Push(5);

    Stack<int>* filtered = st.Where([](int x) { return x % 2 == 0; });

    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->GetSize(), 2);

    EXPECT_EQ(filtered->Pop(), 4);
    EXPECT_EQ(filtered->Pop(), 2);

    delete filtered;
}

TEST(StackTest, ReduceAccumulatesElements) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);
    st.Push(4);

    int sum = st.Reduce<int>(0, [](int acc, int x) { return acc + x; });

    EXPECT_EQ(sum, 10);
}

TEST(StackTest, ContainsSubsequenceWorksForSequence) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);
    st.Push(3);
    st.Push(4);
    st.Push(5);

    int items1[] = {2, 3, 4};
    ListSequence<int> seq1(items1, 3);

    int items2[] = {2, 4};
    ListSequence<int> seq2(items2, 2);

    int items3[] = {1, 2};
    ListSequence<int> seq3(items3, 2);

    EXPECT_TRUE(st.ContainsSubsequence(seq1));
    EXPECT_FALSE(st.ContainsSubsequence(seq2));
    EXPECT_TRUE(st.ContainsSubsequence(seq3));
}

TEST(StackTest, ContainsSubsequenceWorksForStack) {
    Stack<int> st;
    st.Push(10);
    st.Push(20);
    st.Push(30);
    st.Push(40);

    Stack<int> sub1;
    sub1.Push(20);
    sub1.Push(30);

    Stack<int> sub2;
    sub2.Push(30);
    sub2.Push(20);

    EXPECT_TRUE(st.ContainsSubsequence(sub1));
    EXPECT_FALSE(st.ContainsSubsequence(sub2));
}

TEST(StackTest, EmptySubsequenceIsAlwaysContained) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);

    ListSequence<int> emptySeq;
    EXPECT_TRUE(st.ContainsSubsequence(emptySeq));
}

TEST(StackTest, TooLongSubsequenceIsNotContained) {
    Stack<int> st;
    st.Push(1);
    st.Push(2);

    int items[] = {1, 2, 3};
    ListSequence<int> seq(items, 3);

    EXPECT_FALSE(st.ContainsSubsequence(seq));
}