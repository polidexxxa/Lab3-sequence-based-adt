// main.cpp
#include <iostream>
#include <iomanip>
#include <random>
#include <limits>
#include <string>
#include "SquareMatrix.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "Deque.hpp"


void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValidInput() {
    if (std::cin.fail()) {
        clearInputBuffer();
        std::cout << "Invalid input! Please enter a number.\n";
        return false;
    }
    return true;
}

void printSeparator() {
    std::cout << "========================================" << std::endl;
}

void printHeader(const std::string& title) {
    printSeparator();
    std::cout << "     " << title << std::endl;
    printSeparator();
}

// Cлучайное заполнение 

void fillMatrixRandom(SquareMatrix<int>& mat, int min = 0, int max = 10) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    
    for (int i = 0; i < mat.GetSize(); i++) {
        for (int j = 0; j < mat.GetSize(); j++) {
            mat(i, j) = dist(gen);
        }
    }
    std::cout << "Matrix filled with random values" << std::endl;
}

void fillMatrixRandom(SquareMatrix<double>& mat, double min = 0.0, double max = 10.0) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    
    for (int i = 0; i < mat.GetSize(); i++) {
        for (int j = 0; j < mat.GetSize(); j++) {
            mat(i, j) = dist(gen);
        }
    }
    std::cout << "Matrix filled with random values" << std::endl;
}

void fillQueueRandom(Queue<int>& q, int count, int min = 0, int max = 100) {
    q.Clear();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    
    for (int i = 0; i < count; i++) {
        q.Enqueue(dist(gen));
    }
    std::cout << "Queue filled with " << count << " random elements" << std::endl;
}

void fillQueueRandom(Queue<double>& q, int count, double min = 0.0, double max = 100.0) {
    q.Clear();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    
    for (int i = 0; i < count; i++) {
        q.Enqueue(dist(gen));
    }
    std::cout << "Queue filled with " << count << " random elements" << std::endl;
}

// Матрицы

template<typename T>
void printMatrix(const SquareMatrix<T>& mat, const std::string& name = "Matrix") {
    int size = mat.GetSize();
    std::cout << name << " (" << size << "x" << size << "):" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "  ";
        for (int j = 0; j < size; j++) {
            std::cout << std::setw(8) << std::setprecision(4) << mat(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void fillMatrixManually(SquareMatrix<T>& mat) {
    int size = mat.GetSize();
    std::cout << "Enter matrix elements " << size << "x" << size << ":" << std::endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            T val;
            do {
                std::cout << "  mat[" << i << "][" << j << "] = ";
                std::cin >> val;
                if (!isValidInput()) continue;
                break;
            } while (true);
            mat(i, j) = val;
        }
    }
}

template<typename T>
void matrixOperationsMenu(SquareMatrix<T>& mat) {
    int choice;
    do {
        clearScreen();
        printHeader("MATRIX OPERATIONS");
        printMatrix(mat, "Current matrix A");
        
        std::cout << "\nSelect operation:" << std::endl;
        std::cout << "  1. Set element" << std::endl;
        std::cout << "  2. Get element" << std::endl;
        std::cout << "  3. Fill matrix" << std::endl;
        std::cout << "  4. Fill with identity matrix" << std::endl;
        std::cout << "  5. Fill diagonal" << std::endl;
        std::cout << "  6. Add with another matrix" << std::endl;
        std::cout << "  7. Subtract another matrix" << std::endl;
        std::cout << "  8. Multiply by another matrix" << std::endl;
        std::cout << "  9. Multiply by scalar" << std::endl;
        std::cout << " 10. Transpose" << std::endl;
        std::cout << " 11. Swap rows" << std::endl;
        std::cout << " 12. Swap columns" << std::endl;
        std::cout << " 13. Multiply row by scalar" << std::endl;
        std::cout << " 14. Add row to row" << std::endl;
        std::cout << " 15. Compute norms" << std::endl;
        std::cout << " 16. Map (multiply all elements by 2)" << std::endl;
        std::cout << " 17. Where (keep even numbers)" << std::endl;
        std::cout << " 18. Reduce (sum of all elements)" << std::endl;
        std::cout << "  0. Return to main menu" << std::endl;
        std::cout << "Choice: ";
        
        std::cin >> choice;
        if (!isValidInput()) {
            choice = -1;
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    int i, j;
                    T val;
                    do {
                        std::cout << "Enter row: ";
                        std::cin >> i;
                        if (!isValidInput()) continue;
                        std::cout << "Enter col: ";
                        std::cin >> j;
                        if (!isValidInput()) continue;
                        if (i >= 0 && i < mat.GetSize() && j >= 0 && j < mat.GetSize()) break;
                        std::cout << "Index out of range! Try again.\n";
                    } while (true);
                    
                    do {
                        std::cout << "Enter value: ";
                        std::cin >> val;
                        if (isValidInput()) break;
                    } while (true);
                    
                    mat(i, j) = val;
                    std::cout << "Element set" << std::endl;
                    waitForEnter();
                    break;
                }
                case 2: {
                    int i, j;
                    do {
                        std::cout << "Enter row: ";
                        std::cin >> i;
                        if (!isValidInput()) continue;
                        std::cout << "Enter col: ";
                        std::cin >> j;
                        if (!isValidInput()) continue;
                        if (i >= 0 && i < mat.GetSize() && j >= 0 && j < mat.GetSize()) break;
                        std::cout << "Index out of range! Try again.\n";
                    } while (true);
                    
                    std::cout << "Value: " << mat(i, j) << std::endl;
                    waitForEnter();
                    break;
                }
                case 3: {
                    int fillChoice;
                    do {
                        std::cout << "  1. Manual input" << std::endl;
                        std::cout << "  2. Random values" << std::endl;
                        std::cout << "Choice: ";
                        std::cin >> fillChoice;
                        if (!isValidInput()) continue;
                        if (fillChoice == 1 || fillChoice == 2) break;
                        std::cout << "Invalid choice! Enter 1 or 2.\n";
                    } while (true);
                    
                    if (fillChoice == 1) {
                        fillMatrixManually(mat);
                    } else {
                        T min, max;
                        do {
                            std::cout << "Enter min: ";
                            std::cin >> min;
                            if (!isValidInput()) continue;
                            std::cout << "Enter max: ";
                            std::cin >> max;
                            if (!isValidInput()) continue;
                            if (min <= max) break;
                            std::cout << "Min must be <= max!\n";
                        } while (true);
                        fillMatrixRandom(mat, min, max);
                    }
                    break;
                }
                case 4:
                    mat.FillIdentity();
                    std::cout << "Matrix filled with identity" << std::endl;
                    break;
                case 5: {
                    T val;
                    do {
                        std::cout << "Enter value for diagonal: ";
                        std::cin >> val;
                        if (isValidInput()) break;
                    } while (true);
                    mat.FillDiagonal(val);
                    std::cout << "Diagonal filled" << std::endl;
                    break;
                }
                case 6: {
                    SquareMatrix<T> other(mat.GetSize());
                    fillMatrixRandom(other, 0, 5);
                    printMatrix(other, "Matrix B");
                    SquareMatrix<T>* result = mat + other;
                    printMatrix(*result, "A + B");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 7: {
                    SquareMatrix<T> other(mat.GetSize());
                    fillMatrixRandom(other, 0, 5);
                    printMatrix(other, "Matrix B");
                    SquareMatrix<T>* result = mat - other;
                    printMatrix(*result, "A - B");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 8: {
                    SquareMatrix<T> other(mat.GetSize());
                    fillMatrixRandom(other, 0, 5);
                    printMatrix(other, "Matrix B");
                    SquareMatrix<T>* result = mat * other;
                    printMatrix(*result, "A * B");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 9: {
                    T scalar;
                    do {
                        std::cout << "Enter scalar: ";
                        std::cin >> scalar;
                        if (isValidInput()) break;
                    } while (true);
                    SquareMatrix<T>* result = mat * scalar;
                    printMatrix(*result, "A * " + std::to_string(scalar));
                    delete result;
                    waitForEnter();
                    break;
                }
                case 10: {
                    SquareMatrix<T>* result = mat.Transpose();
                    printMatrix(*result, "Transposed matrix");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 11: {
                    int r1, r2;
                    do {
                        std::cout << "Enter row1: ";
                        std::cin >> r1;
                        if (!isValidInput()) continue;
                        std::cout << "Enter row2: ";
                        std::cin >> r2;
                        if (!isValidInput()) continue;
                        if (r1 >= 0 && r1 < mat.GetSize() && r2 >= 0 && r2 < mat.GetSize()) break;
                        std::cout << "Row index out of range!\n";
                    } while (true);
                    mat.SwapRows(r1, r2);
                    std::cout << "Rows swapped" << std::endl;
                    break;
                }
                case 12: {
                    int c1, c2;
                    do {
                        std::cout << "Enter col1: ";
                        std::cin >> c1;
                        if (!isValidInput()) continue;
                        std::cout << "Enter col2: ";
                        std::cin >> c2;
                        if (!isValidInput()) continue;
                        if (c1 >= 0 && c1 < mat.GetSize() && c2 >= 0 && c2 < mat.GetSize()) break;
                        std::cout << "Column index out of range!\n";
                    } while (true);
                    mat.SwapCols(c1, c2);
                    std::cout << "Columns swapped" << std::endl;
                    break;
                }
                case 13: {
                    int row;
                    T scalar;
                    do {
                        std::cout << "Enter row: ";
                        std::cin >> row;
                        if (!isValidInput()) continue;
                        if (row >= 0 && row < mat.GetSize()) break;
                        std::cout << "Row index out of range!\n";
                    } while (true);
                    do {
                        std::cout << "Enter scalar: ";
                        std::cin >> scalar;
                        if (isValidInput()) break;
                    } while (true);
                    mat.MultiplyRow(row, scalar);
                    std::cout << "Row multiplied" << std::endl;
                    break;
                }
                case 14: {
                    int src, dst;
                    T scalar = 1;
                    do {
                        std::cout << "Enter source row: ";
                        std::cin >> src;
                        if (!isValidInput()) continue;
                        std::cout << "Enter target row: ";
                        std::cin >> dst;
                        if (!isValidInput()) continue;
                        if (src >= 0 && src < mat.GetSize() && dst >= 0 && dst < mat.GetSize()) break;
                        std::cout << "Row index out of range!\n";
                    } while (true);
                    std::cout << "Enter scalar (default 1): ";
                    std::cin >> scalar;
                    isValidInput(); // clear if invalid, use default 1
                    mat.AddScaledRowToRow(src, dst, scalar);
                    std::cout << "Operation completed" << std::endl;
                    break;
                }
                case 15: {
                    std::cout << "Matrix norms:" << std::endl;
                    std::cout << "  Max row sum:   " << mat.NormMaxRowSum() << std::endl;
                    std::cout << "  Max col sum:   " << mat.NormMaxColSum() << std::endl;
                    std::cout << "  Frobenius:     " << mat.NormFrobenius() << std::endl;
                    std::cout << "  Max element:   " << mat.NormMax() << std::endl;
                    waitForEnter();
                    break;
                }
                case 16: {
                    auto result = mat.Map<T>([](T x) { return x * 2; });
                    printMatrix(*result, "Map (x2)");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 17: {
                    auto result = mat.Where([](T x) { 
                        return static_cast<int>(x) % 2 == 0; 
                    });
                    printMatrix(*result, "Where (even numbers)");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 18: {
                    T sum = mat.Reduce<T>(T(0), [](T acc, T x) { return acc + x; });
                    std::cout << "Sum of all elements: " << sum << std::endl;
                    waitForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Invalid choice! Please enter 0-18.\n";
                    waitForEnter();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            waitForEnter();
        }
        
    } while (choice != 0);
}


// Очереди (числа)
 
template<typename T>
void printQueue(Queue<T>& q, const std::string& name = "Queue") {
    std::cout << name << " (size: " << q.GetSize() << "): ";
    Queue<T> temp = q;
    while (!temp.IsEmpty()) {
        std::cout << temp.Dequeue() << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void fillQueueManually(Queue<T>& q) {
    int count;
    do {
        std::cout << "Enter number of elements (1-100): ";
        std::cin >> count;
        if (!isValidInput()) continue;
        if (count >= 1 && count <= 100) break;
        std::cout << "Count must be between 1 and 100!\n";
    } while (true);
    
    std::cout << "Enter " << count << " elements: " << std::endl;
    for (int i = 0; i < count; i++) {
        T val;
        do {
            std::cout << "Elem " << (i+1) << ": ";
            std::cin >> val;
            if (isValidInput()) break;
        } while (true);
        q.Enqueue(val);
    }
}

template<typename T>
void queueOperationsMenu(Queue<T>& q) {
    int choice;
    do {
        clearScreen();
        printHeader("QUEUE OPERATIONS");
        printQueue(q, "Current queue");
        
        std::cout << "\nSelect operation:" << std::endl;
        std::cout << "  1. Enqueue (add element)" << std::endl;
        std::cout << "  2. Dequeue (remove element)" << std::endl;
        std::cout << "  3. Peek (view first element)" << std::endl;
        std::cout << "  4. Check if empty" << std::endl;
        std::cout << "  5. Get size" << std::endl;
        std::cout << "  6. Fill queue" << std::endl;
        std::cout << "  7. Clear queue" << std::endl;
        std::cout << "  8. Concat (concatenate with another queue)" << std::endl;
        std::cout << "  9. Merge (append another queue)" << std::endl;
        std::cout << " 10. GetSubqueue (get subqueue)" << std::endl;
        std::cout << " 11. Map (multiply all elements by 2)" << std::endl;
        std::cout << " 12. Where (keep even numbers)" << std::endl;
        std::cout << " 13. Reduce (sum of elements)" << std::endl;
        std::cout << " 14. Split (split by parity)" << std::endl;
        std::cout << " 15. ContainsSubsequence (search for subsequence)" << std::endl;
        std::cout << "  0. Return to main menu" << std::endl;
        std::cout << "Choice: ";
        
        std::cin >> choice;
        if (!isValidInput()) {
            choice = -1;
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    T val;
                    do {
                        std::cout << "Enter value: ";
                        std::cin >> val;
                        if (isValidInput()) break;
                    } while (true);
                    q.Enqueue(val);
                    std::cout << "Element added" << std::endl;
                    break;
                }
                case 2: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty! Cannot dequeue." << std::endl;
                        waitForEnter();
                        break;
                    }
                    T val = q.Dequeue();
                    std::cout << "Removed element: " << val << std::endl;
                    waitForEnter();
                    break;
                }
                case 3: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty!" << std::endl;
                    } else {
                        T val = q.Peek();
                        std::cout << "First element: " << val << std::endl;
                    }
                    waitForEnter();
                    break;
                }
                case 4:
                    std::cout << "Queue is " << (q.IsEmpty() ? "empty" : "not empty") << std::endl;
                    waitForEnter();
                    break;
                case 5:
                    std::cout << "Queue size: " << q.GetSize() << std::endl;
                    waitForEnter();
                    break;
                case 6: {
                    int fillChoice;
                    do {
                        std::cout << "  1. Manual input" << std::endl;
                        std::cout << "  2. Random values" << std::endl;
                        std::cout << "Choice: ";
                        std::cin >> fillChoice;
                        if (!isValidInput()) continue;
                        if (fillChoice == 1 || fillChoice == 2) break;
                        std::cout << "Invalid choice! Enter 1 or 2.\n";
                    } while (true);
                    
                    if (fillChoice == 1) {
                        fillQueueManually(q);
                    } else {
                        int count;
                        T min, max;
                        do {
                            std::cout << "Enter number of elements (1-100): ";
                            std::cin >> count;
                            if (!isValidInput()) continue;
                            if (count >= 1 && count <= 100) break;
                            std::cout << "Count must be between 1 and 100!\n";
                        } while (true);
                        
                        do {
                            std::cout << "Enter min: ";
                            std::cin >> min;
                            if (!isValidInput()) continue;
                            std::cout << "Enter max: ";
                            std::cin >> max;
                            if (!isValidInput()) continue;
                            if (min <= max) break;
                            std::cout << "Min must be <= max!\n";
                        } while (true);
                        
                        fillQueueRandom(q, count, min, max);
                    }
                    break;
                }
                case 7:
                    q.Clear();
                    std::cout << "Queue cleared" << std::endl;
                    break;
                case 8: {
                    Queue<T> other;
                    std::cout << "Enter 3 elements for queue B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        T val;
                        do {
                            std::cout << "Elem " << (i+1) << ": ";
                            std::cin >> val;
                            if (isValidInput()) break;
                        } while (true);
                        other.Enqueue(val);
                    }
                    printQueue(other, "Queue B");
                    Queue<T>* result = q.Concat(other);
                    printQueue(*result, "A + B");
                    waitForEnter();
                    break;
                }
                case 9: {
                    Queue<T> other;
                    std::cout << "Enter 3 elements for queue B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        T val;
                        do {
                            std::cout << "Elem " << (i+1) << ": ";
                            std::cin >> val;
                            if (isValidInput()) break;
                        } while (true);
                        other.Enqueue(val);
                    }
                    printQueue(other, "Queue B");
                    q.Merge(other);
                    std::cout << "Queue B appended to A" << std::endl;
                    break;
                }
                case 10: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty!" << std::endl;
                        waitForEnter();
                        break;
                    }
                    int start, end;
                    do {
                        std::cout << "Enter start index (0-" << q.GetSize() - 1 << "): ";
                        std::cin >> start;
                        if (!isValidInput()) continue;
                        std::cout << "Enter end index (" << start << "-" << q.GetSize() - 1 << "): ";
                        std::cin >> end;
                        if (!isValidInput()) continue;
                        if (start >= 0 && start < q.GetSize() && end >= start && end < q.GetSize()) break;
                        std::cout << "Invalid indices!\n";
                    } while (true);
                    Queue<T>* sub = q.GetSubcontainer(start, end);
                    printQueue(*sub, "Subqueue");
                    waitForEnter();
                    break;
                }
                case 11: {
                    Queue<T>* mapped = q.Map<T>([](T x) { return x * 2; });
                    printQueue(*mapped, "Map (x2)");
                    waitForEnter();
                    break;
                }
                case 12: {
                    Queue<T>* filtered = q.Where([](T x) { 
                        return static_cast<int>(x) % 2 == 0; 
                    });
                    printQueue(*filtered, "Where (even numbers)");
                    waitForEnter();
                    break;
                }
                case 13: {
                    T sum = q.Reduce<T>(T(0), [](T acc, T x) { return acc + x; });
                    std::cout << "Sum of elements: " << sum << std::endl;
                    waitForEnter();
                    break;
                }
                case 14: {
                    auto result = q.Split([](T x) { return static_cast<int>(x) % 2 == 0; });
                    printQueue(result.first, "Even numbers");
                    printQueue(result.second, "Odd numbers");
                    waitForEnter();
                    break;
                }
                case 15: {
                    Queue<T> sub;
                    std::cout << "Enter subsequence (finish with -1): ";
                    T val;
                    while (true) {
                        std::cin >> val;
                        if (std::cin.fail()) {
                            clearInputBuffer();
                            std::cout << "Invalid input! Enter numbers only.\n";
                            continue;
                        }
                        if (val == T(-1)) break;
                        sub.Enqueue(val);
                    }
                    bool contains = q.ContainsSubsequence(sub);
                    std::cout << (contains ? "Contains" : "Does not contain") << std::endl;
                    waitForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Invalid choice! Please enter 0-15.\n";
                    waitForEnter();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            waitForEnter();
        }
        
    } while (choice != 0);
}


// Очередь строк

void printQueue(Queue<std::string>& q, const std::string& name = "Queue") {
    std::cout << name << " (size: " << q.GetSize() << "): ";
    Queue<std::string> temp = q;
    while (!temp.IsEmpty()) {
        std::cout << temp.Dequeue() << " ";
    }
    std::cout << std::endl;
}

void fillQueueManually(Queue<std::string>& q) {
    q.Clear();
    
    int count;
    do {
        std::cout << "Enter number of strings (1-100): ";
        std::cin >> count;
        if (!isValidInput()) continue;
        if (count >= 1 && count <= 100) break;
        std::cout << "Count must be between 1 and 100!\n";
    } while (true);
    
    std::cout << "Enter " << count << " strings: " << std::endl;
    for (int i = 0; i < count; i++) {
        std::string val;
        std::cout << "Elem " << (i+1) << ": ";
        std::cin >> val;
        q.Enqueue(val);
    }
    clearInputBuffer();
}

void queueOperationsMenu(Queue<std::string>& q) {
    int choice;
    do {
        clearScreen();
        printHeader("QUEUE OPERATIONS (STRING)");
        printQueue(q, "Current queue");
        
        std::cout << "\nSelect operation:" << std::endl;
        std::cout << "  1. Enqueue (add string)" << std::endl;
        std::cout << "  2. Dequeue (remove string)" << std::endl;
        std::cout << "  3. Peek (view first string)" << std::endl;
        std::cout << "  4. Check if empty" << std::endl;
        std::cout << "  5. Get size" << std::endl;
        std::cout << "  6. Fill queue" << std::endl;
        std::cout << "  7. Clear queue" << std::endl;
        std::cout << "  8. Concat (concatenate with another queue)" << std::endl;
        std::cout << "  9. Merge (append another queue)" << std::endl;
        std::cout << " 10. GetSubqueue (get subqueue)" << std::endl;
        std::cout << " 11. Map (duplicate each string)" << std::endl;
        std::cout << " 12. Where (keep strings with length > 3)" << std::endl;
        std::cout << " 13. Reduce (concatenate all strings)" << std::endl;
        std::cout << " 14. Split (split by length > 3)" << std::endl;
        std::cout << " 15. ContainsSubsequence (search for subsequence)" << std::endl;
        std::cout << "  0. Return to main menu" << std::endl;
        std::cout << "Choice: ";
        
        std::cin >> choice;
        if (!isValidInput()) {
            choice = -1;
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    std::string val;
                    std::cout << "Enter string: ";
                    std::cin >> val;
                    q.Enqueue(val);
                    std::cout << "String added" << std::endl;
                    break;
                }
                case 2: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty! Cannot dequeue." << std::endl;
                        waitForEnter();
                        break;
                    }
                    std::string val = q.Dequeue();
                    std::cout << "Removed string: " << val << std::endl;
                    waitForEnter();
                    break;
                }
                case 3: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty!" << std::endl;
                    } else {
                        std::string val = q.Peek();
                        std::cout << "First string: " << val << std::endl;
                    }
                    waitForEnter();
                    break;
                }
                case 4:
                    std::cout << "Queue is " << (q.IsEmpty() ? "empty" : "not empty") << std::endl;
                    waitForEnter();
                    break;
                case 5:
                    std::cout << "Queue size: " << q.GetSize() << std::endl;
                    waitForEnter();
                    break;
                case 6:
                    fillQueueManually(q);
                    break;
                case 7:
                    q.Clear();
                    std::cout << "Queue cleared" << std::endl;
                    break;
                case 8: {
                    Queue<std::string> other;
                    std::cout << "Enter 3 strings for queue B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        std::string val;
                        std::cout << "Elem " << (i+1) << ": ";
                        std::cin >> val;
                        other.Enqueue(val);
                    }
                    clearInputBuffer();
                    printQueue(other, "Queue B");
                    Queue<std::string>* result = q.Concat(other);
                    printQueue(*result, "A + B");
                    waitForEnter();
                    break;
                }
                case 9: {
                    Queue<std::string> other;
                    std::cout << "Enter 3 strings for queue B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        std::string val;
                        std::cout << "Elem " << (i+1) << ": ";
                        std::cin >> val;
                        other.Enqueue(val);
                    }
                    clearInputBuffer();
                    printQueue(other, "Queue B");
                    q.Merge(other);
                    std::cout << "Queue B appended to A" << std::endl;
                    break;
                }
                case 10: {
                    if (q.IsEmpty()) {
                        std::cout << "Queue is empty!" << std::endl;
                        waitForEnter();
                        break;
                    }
                    int start, end;
                    do {
                        std::cout << "Enter start index (0-" << q.GetSize() - 1 << "): ";
                        std::cin >> start;
                        if (!isValidInput()) continue;
                        std::cout << "Enter end index (" << start << "-" << q.GetSize() - 1 << "): ";
                        std::cin >> end;
                        if (!isValidInput()) continue;
                        if (start >= 0 && start < q.GetSize() && end >= start && end < q.GetSize()) break;
                        std::cout << "Invalid indices!\n";
                    } while (true);
                    Queue<std::string>* sub = q.GetSubcontainer(start, end);
                    printQueue(*sub, "Subqueue");
                    waitForEnter();
                    break;
                }
                case 11: {
                    Queue<std::string>* mapped = q.Map<std::string>([](const std::string& s) { 
                        return s + s; 
                    });
                    printQueue(*mapped, "Map (duplicate)");
                    waitForEnter();
                    break;
                }
                case 12: {
                    Queue<std::string>* filtered = q.Where([](const std::string& s) { 
                        return s.length() > 3; 
                    });
                    printQueue(*filtered, "Where (length > 3)");
                    waitForEnter();
                    break;
                }
                case 13: {
                    std::string result = q.Reduce<std::string>("", [](std::string acc, const std::string& x) { 
                        return acc + x; 
                    });
                    std::cout << "Concatenated strings: " << result << std::endl;
                    waitForEnter();
                    break;
                }
                case 14: {
                    auto result = q.Split([](const std::string& s) { return s.length() > 3; });
                    printQueue(result.first, "Length > 3");
                    printQueue(result.second, "Length <= 3");
                    waitForEnter();
                    break;
                }
                case 15: {
                    Queue<std::string> sub;
                    std::cout << "Enter subsequence (finish with 'end'): ";
                    std::string val;
                    while (std::cin >> val && val != "end") {
                        sub.Enqueue(val);
                    }
                    clearInputBuffer();
                    bool contains = q.ContainsSubsequence(sub);
                    std::cout << (contains ? "Contains" : "Does not contain") << std::endl;
                    waitForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Invalid choice! Please enter 0-15.\n";
                    waitForEnter();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            waitForEnter();
        }
        
    } while (choice != 0);
}

// стэк 
template<typename T>
void printStack(Stack<T>& s, const std::string& name = "Stack") {
    std::cout << name << " (size: " << s.GetSize() << "): ";
    Stack<T> temp = s;
    Stack<T> reversed;
    
    while (!temp.IsEmpty()) {
        reversed.Push(temp.Pop());
    }
    
    std::cout << "[BOTTOM] ";
    while (!reversed.IsEmpty()) {
        std::cout << reversed.Pop() << " ";
    }
    std::cout << "[TOP]" << std::endl;
}

template<typename T>
void fillStackManually(Stack<T>& s) {
    s.Clear();
    
    int count;
    do {
        std::cout << "Enter number of elements (1-100): ";
        if (!(std::cin >> count)) {
            clearInputBuffer();
            std::cout << "Invalid input!\n";
            continue;
        }
        if (count >= 1 && count <= 100) break;
        std::cout << "Count must be between 1 and 100!\n";
    } while (true);
    
    std::cout << "Enter " << count << " elements (will be pushed in order):" << std::endl;
    for (int i = 0; i < count; i++) {
        T val;
        do {
            std::cout << "Elem " << (i+1) << ": ";
            if (!(std::cin >> val)) {
                clearInputBuffer();
                std::cout << "Invalid value!\n";
                continue;
            }
            break;
        } while (true);
        s.Push(val);
    }
}

void fillStackRandom(Stack<int>& s, int count, int min = 0, int max = 100) {
    s.Clear();
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    for (int i = 0; i < count; i++) {
        s.Push(dist(gen));
    }
    
    std::cout << "Stack filled with " << count << " random elements" << std::endl;
}

template<typename T>
void stackOperationsMenu(Stack<T>& s) {
    int choice;
    do {
        clearScreen();
        printHeader("STACK OPERATIONS");
        printStack(s, "Current stack");
        
        std::cout << "\nSelect operation:" << std::endl;
        std::cout << "  1. Push (add element on top)" << std::endl;
        std::cout << "  2. Pop (remove top element)" << std::endl;
        std::cout << "  3. Peek (view top element)" << std::endl;
        std::cout << "  4. Check if empty" << std::endl;
        std::cout << "  5. Get size" << std::endl;
        std::cout << "  6. Fill stack" << std::endl;
        std::cout << "  7. Clear stack" << std::endl;
        std::cout << "  8. Concat (concatenate with another stack)" << std::endl;
        std::cout << "  9. GetSubstack (get substack)" << std::endl;
        std::cout << " 10. Map (multiply all elements by 2)" << std::endl;
        std::cout << " 11. Where (keep even numbers)" << std::endl;
        std::cout << " 12. Reduce (sum of elements)" << std::endl;
        std::cout << " 13. ContainsSubsequence (search for subsequence)" << std::endl;
        std::cout << "  0. Return to main menu" << std::endl;
        std::cout << "Choice: ";
        
        std::cin >> choice;
        if (!isValidInput()) {
            choice = -1;
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    T val;
                    do {
                        std::cout << "Enter value: ";
                        if (!(std::cin >> val)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        break;
                    } while (true);
                    s.Push(val);
                    std::cout << "Element pushed" << std::endl;
                    break;
                }
                case 2: {
                    if (s.IsEmpty()) {
                        std::cout << "Stack is empty! Cannot pop." << std::endl;
                        waitForEnter();
                        break;
                    }
                    T val = s.Pop();
                    std::cout << "Popped element: " << val << std::endl;
                    waitForEnter();
                    break;
                }
                case 3: {
                    if (s.IsEmpty()) {
                        std::cout << "Stack is empty!" << std::endl;
                    } else {
                        T val = s.Peek();
                        std::cout << "Top element: " << val << std::endl;
                    }
                    waitForEnter();
                    break;
                }
                case 4:
                    std::cout << "Stack is " << (s.IsEmpty() ? "empty" : "not empty") << std::endl;
                    waitForEnter();
                    break;
                case 5:
                    std::cout << "Stack size: " << s.GetSize() << std::endl;
                    waitForEnter();
                    break;
                case 6: {
                    int fillChoice;
                    do {
                        std::cout << "  1. Manual input" << std::endl;
                        std::cout << "  2. Random values" << std::endl;
                        std::cout << "Choice: ";
                        if (!(std::cin >> fillChoice)) {
                            clearInputBuffer();
                            std::cout << "Invalid choice!\n";
                            continue;
                        }
                        if (fillChoice == 1 || fillChoice == 2) break;
                        std::cout << "Invalid choice! Enter 1 or 2.\n";
                    } while (true);
                    
                    if (fillChoice == 1) {
                        fillStackManually(s);
                    } else {
                        int count;
                        T min, max;
                        do {
                            std::cout << "Enter number of elements (1-100): ";
                            if (!(std::cin >> count)) {
                                clearInputBuffer();
                                std::cout << "Invalid count!\n";
                                continue;
                            }
                            if (count >= 1 && count <= 100) break;
                            std::cout << "Count must be between 1 and 100!\n";
                        } while (true);
                        
                        do {
                            std::cout << "Enter min: ";
                            if (!(std::cin >> min)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            std::cout << "Enter max: ";
                            if (!(std::cin >> max)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            if (min <= max) break;
                            std::cout << "Min must be <= max!\n";
                        } while (true);
                        fillStackRandom(s, count, min, max);
                    }
                    break;
                }
                case 7:
                    s.Clear();
                    std::cout << "Stack cleared" << std::endl;
                    break;
                case 8: {
                    Stack<T> other;
                    std::cout << "Enter 3 elements for stack B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        T val;
                        do {
                            std::cout << "Elem " << (i+1) << ": ";
                            if (!(std::cin >> val)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            break;
                        } while (true);
                        other.Push(val);
                    }
                    printStack(other, "Stack B");
                    Stack<T>* result = s.Concat(other);
                    printStack(*result, "A + B");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 9: {
                    if (s.IsEmpty()) {
                        std::cout << "Stack is empty!" << std::endl;
                        waitForEnter();
                        break;
                    }
                    int start, end;
                    do {
                        std::cout << "Enter start index (0-" << s.GetSize() - 1 << "): ";
                        if (!(std::cin >> start)) {
                            clearInputBuffer();
                            std::cout << "Invalid index!\n";
                            continue;
                        }
                        std::cout << "Enter end index (" << start << "-" << s.GetSize() - 1 << "): ";
                        if (!(std::cin >> end)) {
                            clearInputBuffer();
                            std::cout << "Invalid index!\n";
                            continue;
                        }
                        if (start >= 0 && start < s.GetSize() && end >= start && end < s.GetSize()) break;
                        std::cout << "Invalid indices!\n";
                    } while (true);
                    Stack<T>* sub = s.GetSubcontainer(start, end);
                    printStack(*sub, "Substack");
                    delete sub;
                    waitForEnter();
                    break;
                }
                case 10: {
                    Stack<T>* mapped = s.Map<T>([](const T& x) { return x * 2; });
                    printStack(*mapped, "Map (x2)");
                    delete mapped;
                    waitForEnter();
                    break;
                }
                case 11: {
                    Stack<T>* filtered = s.Where([](const T& x) { 
                        return static_cast<int>(x) % 2 == 0; 
                    });
                    printStack(*filtered, "Where (even numbers)");
                    delete filtered;
                    waitForEnter();
                    break;
                }
                case 12: {
                    T sum = s.Reduce<T>(T(0), [](T acc, const T& x) { return acc + x; });
                    std::cout << "Sum of elements: " << sum << std::endl;
                    waitForEnter();
                    break;
                }
                case 13: {
                    Stack<T> sub;
                    std::cout << "Enter subsequence (finish with -1): ";
                    T val;
                    while (true) {
                        if (!(std::cin >> val)) {
                            clearInputBuffer();
                            std::cout << "Invalid input! Enter numbers only.\n";
                            continue;
                        }
                        if (val == T(-1)) break;
                        sub.Push(val);
                    }
                    bool contains = s.ContainsSubsequence(sub);
                    std::cout << (contains ? "Contains" : "Does not contain") << std::endl;
                    waitForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Invalid choice! Please enter 0-13.\n";
                    waitForEnter();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            waitForEnter();
        }
        
    } while (choice != 0);
}

// дек
template<typename T>
void printDeque(Deque<T>& d, const std::string& name = "Deque") {
    std::cout << name << " (size: " << d.GetSize() << "): ";
    Deque<T> temp = d;
    std::cout << "[FRONT] ";
    while (!temp.IsEmpty()) {
        std::cout << temp.PopFront() << " ";
    }
    std::cout << "[BACK]" << std::endl;
}

template<typename T>
void fillDequeManually(Deque<T>& d) {
    d.Clear();
    
    int count;
    do {
        std::cout << "Enter number of elements (1-100): ";
        if (!(std::cin >> count)) {
            clearInputBuffer();
            std::cout << "Invalid input!\n";
            continue;
        }
        if (count >= 1 && count <= 100) break;
        std::cout << "Count must be between 1 and 100!\n";
    } while (true);
    
    std::cout << "Enter " << count << " elements (will be added to back):" << std::endl;
    for (int i = 0; i < count; i++) {
        T val;
        do {
            std::cout << "Elem " << (i+1) << ": ";
            if (!(std::cin >> val)) {
                clearInputBuffer();
                std::cout << "Invalid value!\n";
                continue;
            }
            break;
        } while (true);
        d.PushBack(val);
    }
}

void fillDequeRandom(Deque<int>& d, int count, int min = 0, int max = 100) {
    d.Clear();
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    for (int i = 0; i < count; i++) {
        d.PushBack(dist(gen));
    }
    std::cout << "Deque filled with " << count << " random elements" << std::endl;
}

template<typename T>
void dequeOperationsMenu(Deque<T>& d) {
    int choice;
    do {
        clearScreen();
        printHeader("DEQUE OPERATIONS");
        printDeque(d, "Current deque");
        
        std::cout << "\nSelect operation:" << std::endl;
        std::cout << "  1. PushFront (add element to front)" << std::endl;
        std::cout << "  2. PushBack (add element to back)" << std::endl;
        std::cout << "  3. PopFront (remove front element)" << std::endl;
        std::cout << "  4. PopBack (remove back element)" << std::endl;
        std::cout << "  5. PeekFront (view front element)" << std::endl;
        std::cout << "  6. PeekBack (view back element)" << std::endl;
        std::cout << "  7. Check if empty" << std::endl;
        std::cout << "  8. Get size" << std::endl;
        std::cout << "  9. Fill deque" << std::endl;
        std::cout << " 10. Clear deque" << std::endl;
        std::cout << " 11. Concat (concatenate with another deque)" << std::endl;
        std::cout << " 12. Merge (append another deque)" << std::endl;
        std::cout << " 13. GetSubdeque (get subdeque)" << std::endl;
        std::cout << " 14. MergeSort (sort the deque)" << std::endl;
        std::cout << " 15. IsSorted (check if sorted)" << std::endl;
        std::cout << " 16. Map (multiply all elements by 2)" << std::endl;
        std::cout << " 17. Where (keep even numbers)" << std::endl;
        std::cout << " 18. Reduce (sum of elements)" << std::endl;
        std::cout << " 19. ContainsSubsequence (search for subsequence)" << std::endl;
        std::cout << "  0. Return to main menu" << std::endl;
        std::cout << "Choice: ";
        
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            std::cout << "Invalid input!\n";
            waitForEnter();
            continue;
        }
        
        try {
            switch (choice) {
                case 1: {
                    T val;
                    do {
                        std::cout << "Enter value: ";
                        if (!(std::cin >> val)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        break;
                    } while (true);
                    d.PushFront(val);
                    std::cout << "Element added to front" << std::endl;
                    break;
                }
                case 2: {
                    T val;
                    do {
                        std::cout << "Enter value: ";
                        if (!(std::cin >> val)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        break;
                    } while (true);
                    d.PushBack(val);
                    std::cout << "Element added to back" << std::endl;
                    break;
                }
                case 3: {
                    if (d.IsEmpty()) {
                        std::cout << "Deque is empty! Cannot pop front." << std::endl;
                        waitForEnter();
                        break;
                    }
                    T val = d.PopFront();
                    std::cout << "Popped front element: " << val << std::endl;
                    waitForEnter();
                    break;
                }
                case 4: {
                    if (d.IsEmpty()) {
                        std::cout << "Deque is empty! Cannot pop back." << std::endl;
                        waitForEnter();
                        break;
                    }
                    T val = d.PopBack();
                    std::cout << "Popped back element: " << val << std::endl;
                    waitForEnter();
                    break;
                }
                case 5: {
                    if (d.IsEmpty()) {
                        std::cout << "Deque is empty!" << std::endl;
                    } else {
                        T val = d.PeekFront();
                        std::cout << "Front element: " << val << std::endl;
                    }
                    waitForEnter();
                    break;
                }
                case 6: {
                    if (d.IsEmpty()) {
                        std::cout << "Deque is empty!" << std::endl;
                    } else {
                        T val = d.PeekBack();
                        std::cout << "Back element: " << val << std::endl;
                    }
                    waitForEnter();
                    break;
                }
                case 7:
                    std::cout << "Deque is " << (d.IsEmpty() ? "empty" : "not empty") << std::endl;
                    waitForEnter();
                    break;
                case 8:
                    std::cout << "Deque size: " << d.GetSize() << std::endl;
                    waitForEnter();
                    break;
                case 9: {
                    int fillChoice;
                    do {
                        std::cout << "  1. Manual input" << std::endl;
                        std::cout << "  2. Random values" << std::endl;
                        std::cout << "Choice: ";
                        if (!(std::cin >> fillChoice)) {
                            clearInputBuffer();
                            std::cout << "Invalid choice!\n";
                            continue;
                        }
                        if (fillChoice == 1 || fillChoice == 2) break;
                        std::cout << "Invalid choice! Enter 1 or 2.\n";
                    } while (true);
                    
                    if (fillChoice == 1) {
                        fillDequeManually(d);
                    } else {
                        int count;
                        T min, max;
                        do {
                            std::cout << "Enter number of elements (1-100): ";
                            if (!(std::cin >> count)) {
                                clearInputBuffer();
                                std::cout << "Invalid count!\n";
                                continue;
                            }
                            if (count >= 1 && count <= 100) break;
                            std::cout << "Count must be between 1 and 100!\n";
                        } while (true);
                        
                        do {
                            std::cout << "Enter min: ";
                            if (!(std::cin >> min)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            std::cout << "Enter max: ";
                            if (!(std::cin >> max)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            if (min <= max) break;
                            std::cout << "Min must be <= max!\n";
                        } while (true);
                        fillDequeRandom(d, count, min, max);
                    }
                    break;
                }
                case 10:
                    d.Clear();
                    std::cout << "Deque cleared" << std::endl;
                    break;
                case 11: {
                    Deque<T> other;
                    std::cout << "Enter 3 elements for deque B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        T val;
                        do {
                            std::cout << "Elem " << (i+1) << ": ";
                            if (!(std::cin >> val)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            break;
                        } while (true);
                        other.PushBack(val);
                    }
                    printDeque(other, "Deque B");
                    Deque<T>* result = d.Concat(other);
                    printDeque(*result, "A + B");
                    delete result;
                    waitForEnter();
                    break;
                }
                case 12: {
                    Deque<T> other;
                    std::cout << "Enter 3 elements for deque B: " << std::endl;
                    for (int i = 0; i < 3; i++) {
                        T val;
                        do {
                            std::cout << "Elem " << (i+1) << ": ";
                            if (!(std::cin >> val)) {
                                clearInputBuffer();
                                std::cout << "Invalid value!\n";
                                continue;
                            }
                            break;
                        } while (true);
                        other.PushBack(val);
                    }
                    printDeque(other, "Deque B");
                    d.Merge(other);
                    std::cout << "Deque B merged into A" << std::endl;
                    break;
                }
                case 13: {
                    if (d.IsEmpty()) {
                        std::cout << "Deque is empty!" << std::endl;
                        waitForEnter();
                        break;
                    }
                    int start, end;
                    do {
                        std::cout << "Enter start index (0-" << d.GetSize() - 1 << "): ";
                        if (!(std::cin >> start)) {
                            clearInputBuffer();
                            std::cout << "Invalid index!\n";
                            continue;
                        }
                        std::cout << "Enter end index (" << start << "-" << d.GetSize() - 1 << "): ";
                        if (!(std::cin >> end)) {
                            clearInputBuffer();
                            std::cout << "Invalid index!\n";
                            continue;
                        }
                        if (start >= 0 && start < d.GetSize() && end >= start && end < d.GetSize()) break;
                        std::cout << "Invalid indices!\n";
                    } while (true);
                    Deque<T>* sub = d.GetSubcontainer(start, end);
                    printDeque(*sub, "Subdeque");
                    delete sub;
                    waitForEnter();
                    break;
                }
                case 14:
                    d.MergeSort();
                    std::cout << "Deque sorted" << std::endl;
                    break;
                case 15:
                    std::cout << "Deque is " << (d.IsSorted() ? "sorted" : "not sorted") << std::endl;
                    waitForEnter();
                    break;
                case 16: {
                    Deque<T>* mapped = d.Map<T>([](const T& x) { return x * 2; });
                    printDeque(*mapped, "Map (x2)");
                    delete mapped;
                    waitForEnter();
                    break;
                }
                case 17: {
                    Deque<T>* filtered = d.Where([](const T& x) { 
                        return static_cast<int>(x) % 2 == 0; 
                    });
                    printDeque(*filtered, "Where (even numbers)");
                    delete filtered;
                    waitForEnter();
                    break;
                }
                case 18: {
                    T sum = d.Reduce<T>(T(0), [](T acc, const T& x) { return acc + x; });
                    std::cout << "Sum of elements: " << sum << std::endl;
                    waitForEnter();
                    break;
                }
                case 19: {
                    Deque<T> sub;
                    std::cout << "Enter subsequence (finish with -1): ";
                    T val;
                    while (true) {
                        if (!(std::cin >> val)) {
                            clearInputBuffer();
                            std::cout << "Invalid input! Enter numbers only.\n";
                            continue;
                        }
                        if (val == T(-1)) break;
                        sub.PushBack(val);
                    }
                    bool contains = d.ContainsSubsequence(sub);
                    std::cout << (contains ? "Contains" : "Does not contain") << std::endl;
                    waitForEnter();
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Invalid choice! Please enter 0-19.\n";
                    waitForEnter();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            waitForEnter();
        }
        
    } while (choice != 0);
}

// меню

int main() {
    int choice;
    
    do {
        clearScreen();
        printHeader("LABORATORY WORK #3");
        std::cout << "\nSelect data type:" << std::endl;
        std::cout << "  1. Square Matrix (int)" << std::endl;
        std::cout << "  2. Square Matrix (double)" << std::endl;
        std::cout << "  3. Queue (int)" << std::endl;
        std::cout << "  4. Queue (double)" << std::endl;
        std::cout << "  5. Queue (string)" << std::endl;
        std::cout << "  6. Stack (int)" << std::endl;
        std::cout << "  7. Deque (int)" << std::endl;
        std::cout << "  0. Exit" << std::endl;
        std::cout << "Choice: "; 
        std::cin >> choice;


        if (!isValidInput()) {
            choice = -1;
            continue;
        }
        
        switch (choice) {
            case 1: {
                int size;
                do {
                    std::cout << "Enter matrix size: ";
                    if (!(std::cin >> size)) {
                        clearInputBuffer();
                        std::cout << "Invalid size! Please enter a positive integer.\n";
                        continue;
                    }
                    if (size > 0) break;
                    std::cout << "Size must be positive!\n";
                } while (true);
                SquareMatrix<int> mat(size);
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillMatrixManually(mat);
                } else {
                    int min, max;
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillMatrixRandom(mat, min, max);
                }
                matrixOperationsMenu(mat);
                break;
            }
            case 2: {
                int size;
                do {
                    std::cout << "Enter matrix size: ";
                    if (!(std::cin >> size)) {
                        clearInputBuffer();
                        std::cout << "Invalid size! Please enter a positive integer.\n";
                        continue;
                    }
                    if (size > 0) break;
                    std::cout << "Size must be positive!\n";
                } while (true);
                
                SquareMatrix<double> mat(size);
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillMatrixManually(mat);
                } else {
                    double min, max;
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillMatrixRandom(mat, min, max);
                }
                matrixOperationsMenu(mat);
                break;
            }
            case 3: {
                Queue<int> q;
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillQueueManually(q);
                } else {
                    int count, min, max;
                    do {
                        std::cout << "Enter number of elements (1-100): ";
                        if (!(std::cin >> count)) {
                            clearInputBuffer();
                            std::cout << "Invalid count!\n";
                            continue;
                        }
                        if (count >= 1 && count <= 100) break;
                        std::cout << "Count must be between 1 and 100!\n";
                    } while (true);
                    
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillQueueRandom(q, count, min, max);
                }
                queueOperationsMenu(q);
                break;
            }
            case 4: {
                Queue<double> q;
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillQueueManually(q);
                } else {
                    int count;
                    double min, max;
                    do {
                        std::cout << "Enter number of elements (1-100): ";
                        if (!(std::cin >> count)) {
                            clearInputBuffer();
                            std::cout << "Invalid count!\n";
                            continue;
                        }
                        if (count >= 1 && count <= 100) break;
                        std::cout << "Count must be between 1 and 100!\n";
                    } while (true);
                    
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillQueueRandom(q, count, min, max);
                }
                queueOperationsMenu(q);
                break;
            }
            case 5: {
                Queue<std::string> q;
                fillQueueManually(q);
                queueOperationsMenu(q);
                break;
            }
            case 6: {
                Stack<int> s;
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillStackManually(s);
                } else {
                    int count, min, max;
                    do {
                        std::cout << "Enter number of elements (1-100): ";
                        if (!(std::cin >> count)) {
                            clearInputBuffer();
                            std::cout << "Invalid count!\n";
                            continue;
                        }
                        if (count >= 1 && count <= 100) break;
                        std::cout << "Count must be between 1 and 100!\n";
                    } while (true);
                    
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillStackRandom(s, count, min, max);
                }
                stackOperationsMenu(s);
                break;
            }
            case 7: {
                Deque<int> d;
                int fillChoice;
                do {
                    std::cout << "  1. Manual input" << std::endl;
                    std::cout << "  2. Random values" << std::endl;
                    std::cout << "Choice: ";
                    if (!(std::cin >> fillChoice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice!\n";
                        continue;
                    }
                    if (fillChoice == 1 || fillChoice == 2) break;
                    std::cout << "Invalid choice! Enter 1 or 2.\n";
                } while (true);
                
                if (fillChoice == 1) {
                    fillDequeManually(d);
                } else {
                    int count, min, max;
                    do {
                        std::cout << "Enter number of elements (1-100): ";
                        if (!(std::cin >> count)) {
                            clearInputBuffer();
                            std::cout << "Invalid count!\n";
                            continue;
                        }
                        if (count >= 1 && count <= 100) break;
                        std::cout << "Count must be between 1 and 100!\n";
                    } while (true);
                    
                    do {
                        std::cout << "Enter min: ";
                        if (!(std::cin >> min)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        std::cout << "Enter max: ";
                        if (!(std::cin >> max)) {
                            clearInputBuffer();
                            std::cout << "Invalid value!\n";
                            continue;
                        }
                        if (min <= max) break;
                        std::cout << "Min must be <= max!\n";
                    } while (true);
                    fillDequeRandom(d, count, min, max);
                }
                dequeOperationsMenu(d);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Invalid choice! Please enter 0-5.\n";
                waitForEnter();
                break;
        }
        
    } while (choice != 0);
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}