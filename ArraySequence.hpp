#ifndef _ARRAYSEQUENCE_HPP_
#define _ARRAYSEQUENCE_HPP_

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "Option.hpp"
#include "Exceptions.hpp"
#include <functional>

template<typename T>
class ArraySequence : public Sequence<T> {  //Mutable 
private:
    DynamicArray<T>* items;

public:
    ArraySequence(T* items, int count) {
        this->items = new DynamicArray<T>(items, count);
    }

    ArraySequence() {
        this->items = new DynamicArray<T>(0);
    }

    explicit ArraySequence(const DynamicArray<T>& array) {
        this->items = new DynamicArray<T>(array);
    }

    ArraySequence(const ArraySequence<T>& other) {
        this->items = new DynamicArray<T>(*(other.items));
    }

    ArraySequence(ArraySequence<T>&& other) noexcept 
        : items(other.items) {
        other.items = nullptr;
    }

    explicit ArraySequence(DynamicArray<T>&& array) noexcept 
        : items(new DynamicArray<T>(std::move(array))) {}

    ~ArraySequence() override {
        delete items;
    }

    Sequence<T>* Instance() override { 
        return this;
    }

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*(other.items));
        }
        return *this;
    }

    void AppendInternal(const T& item) override {
        int oldSize = items->GetSize();
        items->Resize(oldSize + 1);
        items->Set(oldSize, item);
    }

    void PrependInternal(const T& item) override {
        int oldSize = items->GetSize();
        items->Resize(oldSize + 1);

        for (int i = oldSize; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
    }

    void InsertAtInternal(const T& item, int index) override {
        if (index < 0 || index > items->GetSize()) {
            throw IndexOutOfRangeException("Index out of range for insertion");
        }

        int oldSize = items->GetSize();
        items->Resize(oldSize + 1);
        
        for (int i = oldSize; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
    }

    T GetFirst() const override {
        if (items->GetSize() == 0) {
            throw EmptyContainerException("Cannot get first element from empty sequence");
        }
        return items->Get(0);
    }

    T GetLast() const override {
        if (items->GetSize() == 0) {
            throw EmptyContainerException("Cannot get last element from empty sequence");
        }
        return items->Get(items->GetSize() - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        DynamicArray<T>* subArray = items->GetSubArray(startIndex, endIndex);
        
        ArraySequence<T>* subsequence = new ArraySequence<T>(*subArray);

        delete subArray;
        return subsequence;
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        if (other == nullptr) {
            throw InvalidArgumentException("Cannot concatenate with null sequence");
        }

        int thisSize = this->GetLength();
        int otherSize = other->GetLength();
        int newSize = thisSize + otherSize;
        
        T* tempArray = new T[newSize];
        
        int pos = 0;

        IEnumerator<T>* enum1 = this->GetEnumerator();
        while (enum1->MoveNext()) {
            tempArray[pos++] = enum1->GetCurrent();
        }
        delete enum1;

        IEnumerator<T>* enum2 = other->GetEnumerator();
        while (enum2->MoveNext()) {
            tempArray[pos++] = enum2->GetCurrent();
        }
        delete enum2;
        
        ArraySequence<T>* result = new ArraySequence<T>(tempArray, newSize);
        delete[] tempArray;
        return result;
    }

    const DynamicArray<T>& GetInternalArray() const {
        return *items;
    }

    template<typename ResultType>
    Sequence<ResultType>* Map(std::function<ResultType(const T&)> func) const {
        ResultType* temp = new ResultType[this->GetLength()];
        
        IEnumerator<T>* enumerator = this->GetEnumerator();
        for (int i = 0; enumerator->MoveNext(); i++) {
            temp[i] = func(enumerator->GetCurrent());
        }
        delete enumerator;

        auto* result = new ArraySequence<ResultType>(temp, this->GetLength());
        delete[] temp;
        return result;
    }

    Sequence<T>* Where(std::function<bool(const T&)> predicate) const {
        int count = 0;
        for (int i = 0; i < this->GetLength(); i++) {
            if (predicate(this->Get(i))) {
                count++;
            }
        }

        if (count == 0) {
            return new ArraySequence<T>();
        }
        
        T* temp = new T[count];
        int index = 0;
        IEnumerator<T>* enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            T val = enumerator->GetCurrent();
            if (predicate(val)) {
                temp[index++] = val;
            }
        }
        delete enumerator;
        
        ArraySequence<T>* result = new ArraySequence<T>(temp, count);
        delete[] temp;
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
        return new ArraySequence<T>(*items);
    }

    Sequence<T>* Slice(int start, int count, Sequence<T>* replacement = nullptr) const override{
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

        int replacementSize = (replacement == nullptr) ? 0 : replacement->GetLength();
        int newSize = length - count + replacementSize;

        T* temp = new T[newSize];
        int pos = 0;
        int index = 0;

        IEnumerator<T>* enumerator = this->GetEnumerator();

        while (enumerator->MoveNext() && index < start) {
            temp[pos++] = enumerator->GetCurrent();
            index++;
        }

        for (int i = 1; i < count && enumerator->MoveNext(); i++) {
            index++;
        }

        if (replacement != nullptr) {
            IEnumerator<T>* replEnum = replacement->GetEnumerator();
            while (replEnum->MoveNext()) {
                temp[pos++] = replEnum->GetCurrent();
            }
            delete replEnum;
        }

        while (enumerator->MoveNext()) {
            temp[pos++] = enumerator->GetCurrent();
        }

        delete enumerator;

        auto* result = new ArraySequence<T>(temp, newSize);
        delete[] temp;
        return result;

    }

    T& operator[](int index) override {
        return items->GetRef(index);
    }
    
    const T& operator[](int index) const override {
        return items->GetRef(index);
    }
    
    IEnumerator<T>* GetEnumerator() const override {
        return new ArrayEnumerator<T>(items);
    }

};


//=============================================================================
//                              IMMUTABLE
//=============================================================================


template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    ImmutableArraySequence() : ArraySequence<T>() {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) 
        : ArraySequence<T>(other) {}
    
    Sequence<T>* Instance() override { 
        return this->Clone(); 
    }

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    T& operator[](int index) override {
        throw UnsupportedOperationException("Immutable sequence does not support mutable access");
    }
    
    using ArraySequence<T>::operator[];
  
};


#endif //_ARRAYSEQUENCE_HPP_