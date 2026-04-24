#ifndef _SQUAREMATRIX_HPP_
#define _SQUAREMATRIX_HPP_

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <functional>

template<typename T>
class SquareMatrix {
private:
    ArraySequence<ArraySequence<T>> rows;
    size_t size;

public:
    SquareMatrix(int n) : size(n) {
        if (n <= 0) {
            throw InvalidArgumentException("Matrix size must be positive");
        }

        for (int i = 0; i < size; i++) {
            DynamicArray<T> rowData(size);
            for (int j = 0; j < size; j++) {
                rowData[j] = T();
            }

            ArraySequence<T> row(std::move(rowData));
            rows.Append(row);
        }
    }

    SquareMatrix(int n, const T& defaultValue) : size(n) {
        if (n <= 0) {
            throw InvalidArgumentException("Matrix size must be positive");
        }

        for (int i = 0; i < size; i++) {
            DynamicArray<T> rowData(size);
            for (int j = 0; j < size; j++) {
                rowData[j] = defaultValue;
            }

            ArraySequence<T> row(std::move(rowData));
            rows.Append(row);
        }
    }

    SquareMatrix(T** data, int n) : size(n) {
        if (n <= 0) {
            throw InvalidArgumentException("Matrix size must be positive");
        }

        for (int i = 0; i < size; i++) {
            ArraySequence<T> row(data[i], size);
            rows.Append(row);
        }
    }

    SquareMatrix(const SquareMatrix& other) : rows(other.rows), size(other.size) {}

    SquareMatrix& operator=(const SquareMatrix& other) {
        if (this != &other) {
            rows = other.rows;
            size = other.size;
        }
        return *this;
    }
    
    ~SquareMatrix() {}


    T Get(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return rows.Get(row).Get(col);
    }

    void Set(int row, int col, const T& value) {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        rows[row][col] = value;
    }

    int GetSize() const {
        return size;
    }


    T& operator()(int row, int col) {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return rows[row][col];
    }

    const T& operator()(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return rows[row][col];
    }

    ArraySequence<T>& operator[](int row) {
        if (row < 0 || row >= size) {
            throw IndexOutOfRangeException("Row index out of range");
        }
        return rows[row];
    }

    const ArraySequence<T>& operator[](int row) const {
        return (*const_cast<SquareMatrix<T>*>(this))[row];
        // if (row < 0 || row >= size) {
        //     throw IndexOutOfRangeException("Row index out of range");
        // }
        // return rows.Get(row);
    }


    SquareMatrix<T>* Add(const SquareMatrix<T>& other) const {
        if (size != other.size) {
            throw DifferentLengthException("Matrix sizes do not match");
        }

        SquareMatrix<T>* result = new SquareMatrix<T>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                (*result)(i, j) = this->Get(i, j) + other.Get(i, j);
            }
        }
        return result;
    }

    SquareMatrix<T>* Subtract(const SquareMatrix<T>& other) const {
        SquareMatrix<T>* negOther = other.MultiplyByScalar(T(-1));
        SquareMatrix<T>* result = this->Add(*negOther);

        delete negOther;
        return result;
    }

    SquareMatrix<T>* MultiplyByScalar(const T& scalar) const {
        SquareMatrix<T>* result = new SquareMatrix<T>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                (*result)(i, j) = this->Get(i, j) * scalar;  //*= copy
            }
        }
        return result;
    }

    SquareMatrix<T>* Multiply(const SquareMatrix<T>& other) const {
        if (size != other.size) {
            throw DifferentLengthException("Matrix sizes do not match");
        }

        SquareMatrix<T>* result = new SquareMatrix<T>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                T sum = T();
                for (int k = 0; k < size; k++) {
                    sum = sum + this->Get(i, k) * other.Get(k, j);
                }
                (*result)(i, j) = sum;
            }
        }
        return result;
    }


    double NormMaxRowSum() const {
        double maxRowSum = 0.0;
        for (int i = 0; i < size; i++) {
            double rowSum = 0.0;
            for (int j = 0; j < size; j++) {
                rowSum += static_cast<double>(std::abs(this->Get(i, j)));
            }
            if (rowSum > maxRowSum) maxRowSum = rowSum;
        }
        return maxRowSum;
    }
    
    double NormMaxColSum() const {
        double maxColSum = 0.0;
        for (int j = 0; j < size; j++) {
            double colSum = 0.0;
            for (int i = 0; i < size; i++) {
                colSum += static_cast<double>(std::abs(this->Get(i, j)));
            }
            if (colSum > maxColSum) maxColSum = colSum;
        }
        return maxColSum;
    }
    
    double NormFrobenius() const {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double val = static_cast<double>(std::abs(this->Get(i, j)));
                sum += val * val;
            }
        }
        return std::sqrt(sum);
    }
    
    double NormMax() const {
        double maxVal = 0.0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double val = static_cast<double>(std::abs(this->Get(i, j)));
                if (val > maxVal) maxVal = val;
            }
        }
        return maxVal;
    }


    void SwapRows(int row1, int row2) {
        if (row1 < 0 || row1 >= size || row2 < 0 || row2 >= size) {
            throw IndexOutOfRangeException("Row index out of range");
        }
        if (row1 == row2) return;
        ArraySequence<T> temp = rows.Get(row1);
        rows[row1] = rows.Get(row2);
        rows[row2] = temp;
    }

    void MultiplyRow(int row, const T& scalar) {
        if (row < 0 || row >= size) {
            throw IndexOutOfRangeException("Row index out of range");
        }
        for (int j = 0; j < size; j++) {
            rows[row][j] = rows.Get(row).Get(j) * scalar;
        }
    }

    void AddScaledRowToRow(int sourceRow, int targetRow, const T& scale = 1) {
        if (sourceRow < 0 || sourceRow >= size || targetRow < 0 || targetRow >= size) {
            throw IndexOutOfRangeException("Row index out of range");
        }
        for (int j = 0; j < size; j++) {
            rows[targetRow][j] = rows.Get(targetRow).Get(j) + rows.Get(sourceRow).Get(j) * scale;
        }
    }

    void AddRowToRow(int sourceRow, int targetRow) {
        AddScaledRowToRow(sourceRow, targetRow, T(1));
    }


    void SwapCols(int col1, int col2) {
        if (col1 < 0 || col1 >= size || col2 < 0 || col2 >= size) {
            throw IndexOutOfRangeException("Column index out of range");
        }
        if (col1 == col2) return;
        for (int i = 0; i < size; i++) {
            T temp = rows.Get(i).Get(col1);
            rows[i][col1] = rows.Get(i).Get(col2);
            rows[i][col2] = temp;
        }
    }

    void MultiplyCol(int col, const T& scalar) {
        if (col < 0 || col >= size) {
            throw IndexOutOfRangeException("Column index out of range");
        }
        for (int i = 0; i < size; i++) {
            rows[i][col] = rows.Get(i).Get(col) * scalar;
        }
    }

    void AddScaledColToCol(int sourceCol, int targetCol, const T& scale = 1) {
        if (sourceCol < 0 || sourceCol >= size || targetCol < 0 || targetCol >= size) {
            throw IndexOutOfRangeException("Column index out of range");
        }
        for (int i = 0; i < size; i++) {
            rows[i][targetCol] = rows.Get(i).Get(targetCol) + rows.Get(i).Get(sourceCol) * scale;
        }
    }

    void AddColToCol(int sourceCol, int targetCol) {
        AddScaledColToCol(sourceCol, targetCol, T(1));
    }


    SquareMatrix<T>* Transpose() const {
        SquareMatrix<T>* result = new SquareMatrix<T>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                (*result)(j, i) = this->Get(i, j);
            }
        }
        return result;
    }

    void Fill(const T& value) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rows[i][j] = value;
            }
        }
    }

    void FillIdentity() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rows[i][j] = (i == j) ? T(1) : T(0);
            }
        }
    }

    void FillDiagonal(const T& value) {
        for (int i = 0; i < size; i++) {
            rows[i][i] = value;
        }
    }

    template<typename ResultType>
    SquareMatrix<ResultType>* Map(std::function<ResultType(const T&)> func) const {
        SquareMatrix<ResultType>* result = new SquareMatrix<ResultType>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                (*result)(i, j) = func(this->Get(i, j));
            }
        }
        return result;
    }

    SquareMatrix<T>* Where(std::function<bool(const T&)> predicate) const {
        SquareMatrix<T>* result = new SquareMatrix<T>(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                T val = this->Get(i, j);
                (*result)(i, j) = predicate(val) ? val : T(0);
            }
        }
        return result;
    }

    template<typename Accumulator>
    Accumulator Reduce(const Accumulator& initial, std::function<Accumulator(Accumulator, const T&)> func) const {
        Accumulator result = initial;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result = func(result, this->Get(i, j));
            }
        }
        return result;
    }

    SquareMatrix<T>* operator+(const SquareMatrix<T>& other) const {
        return this->Add(other);
    }

    SquareMatrix<T>* operator-(const SquareMatrix<T>& other) const {
        return this->Subtract(other);
    }
    
    SquareMatrix<T>* operator*(const SquareMatrix<T>& other) const {
        return this->Multiply(other);
    }
    
    SquareMatrix<T>* operator*(const T& scalar) const {
        return this->MultiplyByScalar(scalar);
    }

    SquareMatrix<T>& operator+=(const SquareMatrix<T>& other) {
        if (size != other.size) {
            throw DifferentLengthException("Matrix sizes do not match");
        }
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rows[i][j] = rows[i][j] + other.Get(i, j);
            }
        }
        return *this;
    }

    SquareMatrix<T>& operator-=(const SquareMatrix<T>& other) {
        if (size != other.size) {
            throw DifferentLengthException("Matrix sizes do not match");
        }
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rows[i][j] = rows[i][j] - other.Get(i, j);
            }
        }
        return *this;
    }

    SquareMatrix<T>& operator*=(const SquareMatrix<T>& other) {
        if (size != other.size) {
            throw DifferentLengthException("Matrix sizes do not match");
        }
        SquareMatrix<T> temp(size);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                T sum = T();
                for (int k = 0; k < size; k++) {
                    sum = sum + rows[i][k] * other.Get(k, j);
                }
                temp(i, j) = sum;
            }
        }

        *this = std::move(temp);
        return *this;
    }

    SquareMatrix<T>& operator*=(const T& scalar) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rows[i][j] = rows[i][j] * scalar;
            }
        }
        return *this;
    }
    
};


#endif //_SQUAREMATRIX_HPP_