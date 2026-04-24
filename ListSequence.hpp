#ifndef _LISTSEQUENCE_HPP_
#define _LISTSEQUENCE_HPP_

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Option.hpp"
#include "Exceptions.hpp"
#include <functional>

template<typename T>
class ListSequence : public Sequence<T> {   //Mutable
private:
    LinkedList<T>* list;

public:
    ListSequence(T* items, int count) {
        this->list = new LinkedList<T>(items, count);
    }

    ListSequence() {
        this->list = new LinkedList<T>();
    }

    explicit ListSequence(const LinkedList<T>& linkedList) {
        this->list = new LinkedList<T>(linkedList);
    }

    ListSequence(const ListSequence<T>& other) {
        this->list = new LinkedList<T>(*(other.list));
    }

    ListSequence(ListSequence<T>&& other) noexcept 
        : list(other.list) {
        other.list = nullptr;
    }

    explicit ListSequence(LinkedList<T>&& linkedList) noexcept 
        : list(new LinkedList<T>(std::move(linkedList))) {}

    ~ListSequence() override {
        delete list;
    }

    Sequence<T>* Instance() override { 
        return this;
    }

    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this != &other) {
            delete list;
            list = new LinkedList<T>(*(other.list));
        }
        return *this;
    }

    T GetFirst() const override {
        return list->GetFirst();
    }

    T GetLast() const override {
        return list->GetLast();
    }

    T Get(int index) const override {
        return list->Get(index);
    }

    int GetLength() const override {
        return list->GetLength();
    }

    void AppendInternal(const T& item) override { 
        list->Append(item); 
    }

    void PrependInternal(const T& item) override { 
        list->Prepend(item); 
    }

    void InsertAtInternal(const T& item, int index) override { 
        list->InsertAt(item, index); 
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);

        ListSequence<T>* subsequence = new ListSequence<T>(*subList);
        
        delete subList;
        return subsequence;
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        if (other == nullptr) {
            throw InvalidArgumentException("Cannot concatenate with null sequence");
        }

        LinkedList<T> resultList;
        
        IEnumerator<T>* enum1 = this->GetEnumerator();
        while (enum1->MoveNext()) {
            resultList.Append(enum1->GetCurrent());
        }
        delete enum1;
        
        IEnumerator<T>* enum2 = other->GetEnumerator();
        while (enum2->MoveNext()) {
            resultList.Append(enum2->GetCurrent());
        }
        delete enum2;
        
        return new ListSequence<T>(resultList);
    }

    const LinkedList<T>& GetInternalList() const {
        return *list;
    }

    template<typename ResultType>
    Sequence<ResultType>* Map(std::function<ResultType(const T&)> func) const {
        ResultType* temp = new ResultType[this->GetLength()];

        IEnumerator<T>* enumerator = this->GetEnumerator();
        for (int i = 0; enumerator->MoveNext(); i++) {
            temp[i] = func(enumerator->GetCurrent());
        }
        delete enumerator;

        auto* result = new ListSequence<ResultType>(temp, this->GetLength());
        delete[] temp;
        return result;
    }

    Sequence<T>* Where(std::function<bool(const T&)> predicate) const {
        ListSequence<T>* result = new ListSequence<T>(); 

        IEnumerator<T>* enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            T val = enumerator->GetCurrent();
            if (predicate(val)) {
                result->Append(val);
            }
        }
        delete enumerator;
    
        return result;
    }

    template<typename Accumulator>
    Accumulator Reduce(const Accumulator& initial, std::function<Accumulator(Accumulator, const T&)> func) const {
        Accumulator result = initial;

        IEnumerator<T>* enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            result = func(result, enumerator->GetCurrent());
        }
        delete enumerator;

        return result;
    }

    Option<T> TryFind(std::function<bool(const T&)> predicate) const override {
        for (int i = 0; i < this->GetLength(); i++) {
            T val = this->Get(i);
            if (predicate(val)) {
                return Option<T>(val);
            }
        }
        return Option<T>();
    }
    
    Option<T> TryGetFirst() const override {
        if (this->GetLength() == 0) {
            return Option<T>();
        }
        return Option<T>(this->GetFirst());
    }
    
    Option<T> TryGetLast() const override {
        if (this->GetLength() == 0) {
            return Option<T>();
        }
        return Option<T>(this->GetLast());
    }

    Sequence<T>* Clone() const override {
        return new ListSequence<T>(*this);
    }

    Sequence<T>* Slice(int start, int count, Sequence<T>* replacement = nullptr) const override {
        int length = this->GetLength();
        
        if (start < 0) {
            start = length + start;
        }
        
        if (start < 0 || start >= length) {
            throw IndexOutOfRangeException("Start index out of range");
        }
        
        if (count < 0) {
            throw InvalidArgumentException("Count cannot be negative");
        }
        
        if (start + count > length) {
            count = length - start;
        }
        
        ListSequence<T>* result = new ListSequence<T>();

        IEnumerator<T>* enumerator = this->GetEnumerator();
        int index = 0;
        
        while (enumerator->MoveNext() && index < start) {
            result->Append(enumerator->GetCurrent());
            index++;
        }

        for (int i = 1; i < count && enumerator->MoveNext(); i++) {
            index++;
        }
        
        if (replacement != nullptr) {
            IEnumerator<T>* replEnum = replacement->GetEnumerator();
            while (replEnum->MoveNext()) {
                result->Append(replEnum->GetCurrent());
            }
            delete replEnum;
        }
        
        while (enumerator->MoveNext()) {
            result->Append(enumerator->GetCurrent());
        }
        
        delete enumerator;
        return result;
    }

    T& operator[](int index) override {
        return list->GetRef(index);
    }
    
    const T& operator[](int index) const override {
        return list->GetRef(index);
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new ListEnumerator<T>(list);
    }

}; 

//=============================================================================
//                              IMMUTABLE
//=============================================================================

template<typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;
    
    ImmutableListSequence() : ListSequence<T>() {}

    ImmutableListSequence(const ImmutableListSequence<T>& other) 
        : ListSequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* Instance() override { return this->Clone(); }
    
    T& operator[](int index) override {
        throw UnsupportedOperationException("Immutable sequence does not support mutable iterators");
    }
    
    using ListSequence<T>::operator[];

};

#endif //_LISTSEQUENCE_HPP_