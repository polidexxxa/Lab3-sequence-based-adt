#ifndef _DEQUE_HPP_
#define _DEQUE_HPP_

#include "ListBasedContainer.hpp"

template <typename T>
class Deque : public ListBasedContainer<T, Deque> {
private:
    using Base = ListBasedContainer<T, Deque>;

    Deque<T>* MergeSortHelper(int left, int right) {
        if (left == right) {
            Deque<T>* result = new Deque<T>();
            result->PushBack(this->storage.Get(left));
            return result;
        }

        int mid = left + (right - left) / 2;
        Deque<T>* leftPart = MergeSortHelper(left, mid);
        Deque<T>* rightPart = MergeSortHelper(mid + 1, right);
        Deque<T>* merged = SortedMerge(leftPart, rightPart);

        delete leftPart;
        delete rightPart;

        return merged;
    }

    Deque<T>* SortedMerge(Deque<T>* left, Deque<T>* right) {
        Deque<T>* result = new Deque<T>();

        while (!left->IsEmpty() && !right->IsEmpty()) {
            if (left->PeekFront() < right->PeekFront()) {
                result->PushBack(left->PopFront());
            } 
            else {
                result->PushBack(right->PopFront());
            }
        }

        while (!left->IsEmpty()) {
            result->PushBack(left->PopFront());
        }

        while (!right->IsEmpty()) {
            result->PushBack(right->PopFront());
        }

        return result;
    }

public:
    Deque() {}
    Deque(const Deque<T>& other) : Base(other) {}
    explicit Deque(const Sequence<T>& seq) : Base(seq) {}

    Deque<T>& operator=(const Deque<T>& other) {
        Base::operator=(other);
        return *this;
    }

    void PushFront(T item) {
        this->storage.Prepend(item);
    }

    void PushBack(T item) {
        this->storage.Append(item);
    }

    T PopFront() {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot pop from empty deque");
        }

        T front = this->storage.GetFirst();
        this->RemoveFrontInternal();
        return front;
    }

    T PopBack() {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot pop from empty deque");
        }

        T back = this->storage.GetLast();
        this->RemoveBackInternal();
        return back;
    }

    T PeekFront() const {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot get front of empty deque");
        }
        return this->storage.GetFirst();
    }

    T PeekBack() const {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot get back of empty deque");
        }
        return this->storage.GetLast();
    }

    Deque<T>& Merge(Deque<T>& other) {
        while (!other.IsEmpty()) {
            PushBack(other.PopFront());
        }
        return *this;
    }

    void MergeSort() {
        if (this->GetSize() <= 1) return;

        Deque<T>* sorted = MergeSortHelper(0, this->GetSize() - 1);
        this->storage = sorted->GetStorage();
        delete sorted;
    }

    bool IsSorted() const {
        int n = this->GetSize();
        if (n <= 1) return true;

        IEnumerator<T>* enumerator = this->storage.GetEnumerator();
        enumerator->MoveNext();
        T prev = enumerator->GetCurrent();

        while (enumerator->MoveNext()) {
            T current = enumerator->GetCurrent();
            if (current < prev) {
                delete enumerator;
                return false;
            }
            prev = current;
        }

        delete enumerator;
        return true;
    }

    using Base::IsEmpty;
    using Base::GetSize;
    using Base::Clear;
    using Base::Concat;
    using Base::GetSubcontainer;
    using Base::Map;
    using Base::Where;
    using Base::Reduce;
    using Base::ContainsSubsequence;
};

#endif //_DEQUE_HPP_