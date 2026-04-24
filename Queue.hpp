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

    bool IsEmpty() const { return Base::IsEmpty(); }
    int GetSize() const { return Base::GetSize(); }
    void Clear() { Base::Clear(); }
    
    Queue<T>* Concat(const Queue<T>& other) const {
        return Base::Concat(other);
    }
    
    Queue<T>* GetSubcontainer(int start, int end) const {
        return Base::GetSubcontainer(start, end);
    }
    
    template <typename ResultType>
    Queue<ResultType>* Map(std::function<ResultType(T)> func) const {
        return Base::template Map<ResultType>(func);
    }
    
    Queue<T>* Where(std::function<bool(const T&)> predicate) const {
        return Base::Where(predicate);
    }
    
    template <typename Accumulator>
    Accumulator Reduce(const Accumulator& initial, std::function<Accumulator(Accumulator, T)> func) const {
        return Base::Reduce(initial, func);
    }
    
    bool ContainsSubsequence(const Sequence<T>& subsequence) const {
        return Base::ContainsSubsequence(subsequence);
    }
    
    bool ContainsSubsequence(const Queue<T>& subsequence) const {
        return Base::ContainsSubsequence(subsequence);
    }
};

#endif //_QUEUE_HPP_