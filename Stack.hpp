#ifndef _STACK_HPP_
#define _STACK_HPP_

#include "ListBasedContainer.hpp"

template <typename T>
class Stack : public ListBasedContainer<T, Stack> {
private:
    using Base = ListBasedContainer<T, Stack>;

public:
    Stack() {}
    Stack(const Stack<T>& other) : Base(other) {}
    explicit Stack(const Sequence<T>& seq) : Base(seq) {}

    Stack<T>& operator=(const Stack<T>& other) {
        Base::operator=(other);
        return *this;
    }

    void Push(T item) {
        this->storage.Append(item);
    }

    T Pop() {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot pop from empty stack");
        }

        T top = this->storage.GetLast();
        this->RemoveBackInternal();
        return top;
    }

    T Peek() const {
        if (this->IsEmpty()) {
            throw EmptyContainerException("Cannot peek into empty stack");
        }

        return this->storage.GetLast();
    }

    bool IsEmpty() const { return Base::IsEmpty(); }
    int GetSize() const { return Base::GetSize(); }
    void Clear() { Base::Clear(); }
    
    Stack<T>* Concat(const Stack<T>& other) const {
        return Base::Concat(other);
    }
    
    Stack<T>* GetSubcontainer(int start, int end) const {
        return Base::GetSubcontainer(start, end);
    }
    
    template <typename ResultType>
    Stack<ResultType>* Map(std::function<ResultType(T)> func) const {
        return Base::template Map<ResultType>(func);
    }
    
    Stack<T>* Where(std::function<bool(const T&)> predicate) const {
        return Base::Where(predicate);
    }
    
    template <typename Accumulator>
    Accumulator Reduce(const Accumulator& initial, std::function<Accumulator(Accumulator, T)> func) const {
        return Base::Reduce(initial, func);
    }
    
    bool ContainsSubsequence(const Sequence<T>& subsequence) const {
        return Base::ContainsSubsequence(subsequence);
    }
    
    bool ContainsSubsequence(const Stack<T>& subsequence) const {
        return Base::ContainsSubsequence(subsequence);
    }
};

#endif //_STACK_HPP_