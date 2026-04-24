#ifndef _SEQUENCE_HPP_
#define _SEQUENCE_HPP_

#include "Exceptions.hpp"
#include "Option.hpp"
#include "IEnumerator.hpp"
#include <functional>

template<typename T>
class Sequence {
public:
    virtual ~Sequence() = default;
     
    virtual Sequence<T>* Instance() = 0;
    
    Sequence<T>* Append(T item) {
        Sequence<T>* target = Instance();
        target->AppendInternal(item);
        return target;
    }

    Sequence<T>* Prepend(T item) {
        Sequence<T>* target = Instance();
        target->PrependInternal(item);
        return target;
    }

    Sequence<T>* InsertAt(T item, int index) {
        Sequence<T>* target = Instance();
        target->InsertAtInternal(item, index);
        return target;
    }
    
    virtual void AppendInternal(const T& item) = 0;
    virtual void PrependInternal(const T& item) = 0;
    virtual void InsertAtInternal(const T& item, int index) = 0;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;

    virtual Option<T> TryFind(std::function<bool(const T&)> predicate) const = 0;
    virtual Option<T> TryGetFirst() const = 0;
    virtual Option<T> TryGetLast() const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

    virtual Sequence<T>* Slice(int start, int count, Sequence<T>* replacement = nullptr) const = 0;

    virtual Sequence<T>* Clone() const = 0;

    virtual IEnumerator<T>* GetEnumerator() const = 0;
};

#endif //_SEQUENCE_HPP_