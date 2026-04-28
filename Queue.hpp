#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include "ListBasedContainer.hpp"
#include "Pair.hpp"

template <typename T>
class Queue : public ListBasedContainer<T, Queue> {
private:
    using Base = ListBasedContainer<T, Queue>;

public:
    Queue() {}
    Queue(const Queue<T>& other) : Base(other) {}
    explicit Queue(const Sequence<T>& seq) : Base(seq) {}

    Queue<T>& operator=(const Queue<T>& other) {
        Base::operator=(other);
        return *this;
    }

    void Enqueue(T item) {
        this->storage.Append(item);
    }

    T Dequeue() {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot dequeue from empty queue");
        }

        T front = this->storage.GetFirst();
        this->RemoveFrontInternal();
        return front;
    }

    T Peek() const {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot peek into empty queue");
        }

        return this->storage.GetFirst();
    }

    Queue<T>& Merge(Queue<T>& other) {
        while (!other.IsEmpty()) {
            Enqueue(other.Dequeue());
        }
        return *this;
    }

    Pair<Queue<T>, Queue<T>> Split(std::function<bool(const T&)> predicate) const {
        Queue<T> trueQueue;
        Queue<T> falseQueue;

        IEnumerator<T>* enumerator = this->storage.GetEnumerator();
        while (enumerator->MoveNext()) {
            T value = enumerator->GetCurrent();
            if (predicate(value)) {
                trueQueue.Enqueue(value);
            } 
            else {
                falseQueue.Enqueue(value);
            }
        }
        delete enumerator;

        return Pair<Queue<T>, Queue<T>>(trueQueue, falseQueue);
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

#endif //_QUEUE_HPP_