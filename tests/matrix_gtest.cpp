#include <gtest/gtest.h>
#include <cmath>
#include "SquareMatrix.hpp"
#include "Exceptions.hpp"

static bool DoubleEqual(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

TEST(MatrixTest, DefaultConstructorInitializesZeroMatrix) {
    SquareMatrix<int> mat(3);

    EXPECT_EQ(mat.GetSize(), 3);
    EXPECT_EQ(mat.Get(0, 0), 0);
    EXPECT_EQ(mat.Get(2, 2), 0);
}

TEST(MatrixTest, ConstructorWithDefaultValueFillsMatrix) {
    SquareMatrix<int> mat(3, 5);

    EXPECT_EQ(mat.GetSize(), 3);
    EXPECT_EQ(mat.Get(0, 0), 5);
    EXPECT_EQ(mat.Get(2, 2), 5);
}

TEST(MatrixTest, ConstructorFrom2DArrayCopiesData) {
    int** data = new int*[2];
    data[0] = new int[2]{1, 2};
    data[1] = new int[2]{3, 4};

    SquareMatrix<int> mat(data, 2);

    EXPECT_EQ(mat.Get(0, 0), 1);
    EXPECT_EQ(mat.Get(0, 1), 2);
    EXPECT_EQ(mat.Get(1, 0), 3);
    EXPECT_EQ(mat.Get(1, 1), 4);

    delete[] data[0];
    delete[] data[1];
    delete[] data;
}

TEST(MatrixTest, CopyConstructorCreatesIndependentCopy) {
    SquareMatrix<int> original(2);
    original(0, 0) = 1;
    original(0, 1) = 2;
    original(1, 0) = 3;
    original(1, 1) = 4;

    SquareMatrix<int> copy(original);
    copy(0, 0) = 100;

    EXPECT_EQ(original.Get(0, 0), 1);
    EXPECT_EQ(copy.Get(0, 0), 100);
}

TEST(MatrixTest, GetSetAndBoundsCheckWork) {
    SquareMatrix<int> mat(3);
    mat.Set(0, 0, 1);
    mat.Set(0, 1, 2);
    mat.Set(1, 0, 3);
    mat.Set(2, 2, 5);

    EXPECT_EQ(mat.Get(0, 0), 1);
    EXPECT_EQ(mat.Get(0, 1), 2);
    EXPECT_EQ(mat.Get(1, 0), 3);
    EXPECT_EQ(mat.Get(2, 2), 5);

    EXPECT_THROW(mat.Get(3, 0), IndexOutOfRangeException);
    EXPECT_THROW(mat.Set(3, 0, 1), IndexOutOfRangeException);
}

TEST(MatrixTest, ParenthesesOperatorWorks) {
    SquareMatrix<int> mat(2);
    mat(0, 0) = 1;
    mat(0, 1) = 2;
    mat(1, 0) = 3;
    mat(1, 1) = 4;

    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 1), 2);
    EXPECT_EQ(mat(1, 0), 3);
    EXPECT_EQ(mat(1, 1), 4);
}

TEST(MatrixTest, BracketsOperatorWorks) {
    SquareMatrix<int> mat(2);
    (mat[0])[0] = 1;
    (mat[0])[1] = 2;
    (mat[1])[0] = 3;
    (mat[1])[1] = 4;

    EXPECT_EQ(mat.Get(0, 0), 1);
    EXPECT_EQ(mat.Get(0, 1), 2);
    EXPECT_EQ(mat.Get(1, 0), 3);
    EXPECT_EQ(mat.Get(1, 1), 4);
}

TEST(MatrixTest, AdditionWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;

    SquareMatrix<int>* C = A + B;

    ASSERT_NE(C, nullptr);
    EXPECT_EQ(C->Get(0, 0), 6);
    EXPECT_EQ(C->Get(0, 1), 8);
    EXPECT_EQ(C->Get(1, 0), 10);
    EXPECT_EQ(C->Get(1, 1), 12);

    delete C;
}

TEST(MatrixTest, SubtractionWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 10; A(0, 1) = 20;
    A(1, 0) = 30; A(1, 1) = 40;

    B(0, 0) = 1; B(0, 1) = 2;
    B(1, 0) = 3; B(1, 1) = 4;

    SquareMatrix<int>* C = A - B;

    ASSERT_NE(C, nullptr);
    EXPECT_EQ(C->Get(0, 0), 9);
    EXPECT_EQ(C->Get(0, 1), 18);
    EXPECT_EQ(C->Get(1, 0), 27);
    EXPECT_EQ(C->Get(1, 1), 36);

    delete C;
}

TEST(MatrixTest, ScalarMultiplicationWorks) {
    SquareMatrix<int> A(2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    SquareMatrix<int>* C = A * 2;

    ASSERT_NE(C, nullptr);
    EXPECT_EQ(C->Get(0, 0), 2);
    EXPECT_EQ(C->Get(0, 1), 4);
    EXPECT_EQ(C->Get(1, 0), 6);
    EXPECT_EQ(C->Get(1, 1), 8);

    delete C;
}

TEST(MatrixTest, MatrixMultiplicationWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    B(0, 0) = 2; B(0, 1) = 0;
    B(1, 0) = 1; B(1, 1) = 2;

    SquareMatrix<int>* C = A * B;

    ASSERT_NE(C, nullptr);
    EXPECT_EQ(C->Get(0, 0), 4);
    EXPECT_EQ(C->Get(0, 1), 4);
    EXPECT_EQ(C->Get(1, 0), 10);
    EXPECT_EQ(C->Get(1, 1), 8);

    delete C;
}

TEST(MatrixTest, NormsAreComputedCorrectly) {
    SquareMatrix<int> A(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            A(i, j) = i * 3 + j + 1;
        }
    }

    EXPECT_TRUE(DoubleEqual(A.NormMaxRowSum(), 24.0));
    EXPECT_TRUE(DoubleEqual(A.NormMaxColSum(), 18.0));
    EXPECT_TRUE(DoubleEqual(A.NormFrobenius(), std::sqrt(285.0)));
    EXPECT_TRUE(DoubleEqual(A.NormMax(), 9.0));
}

TEST(MatrixTest, RowOperationsWork) {
    SquareMatrix<int> A(2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    A.SwapRows(0, 1);
    EXPECT_EQ(A.Get(0, 0), 3);
    EXPECT_EQ(A.Get(1, 0), 1);

    A.MultiplyRow(1, 2);
    EXPECT_EQ(A.Get(1, 0), 2);
    EXPECT_EQ(A.Get(1, 1), 4);

    A.AddScaledRowToRow(1, 0, 3);
    EXPECT_EQ(A.Get(0, 0), 3 + 3 * 2);
    EXPECT_EQ(A.Get(0, 1), 4 + 3 * 4);
}

TEST(MatrixTest, ColumnOperationsWork) {
    SquareMatrix<int> A(2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    A.SwapCols(0, 1);
    EXPECT_EQ(A.Get(0, 0), 2);
    EXPECT_EQ(A.Get(1, 0), 4);

    A.MultiplyCol(1, 2);
    EXPECT_EQ(A.Get(0, 1), 2);
    EXPECT_EQ(A.Get(1, 1), 6);

    A.AddScaledColToCol(1, 0, 3);
    EXPECT_EQ(A.Get(0, 0), 2 + 3 * 2);
    EXPECT_EQ(A.Get(1, 0), 4 + 3 * 6);
}

TEST(MatrixTest, TransposeWorks) {
    SquareMatrix<int> A(2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    SquareMatrix<int>* B = A.Transpose();

    ASSERT_NE(B, nullptr);
    EXPECT_EQ(B->Get(0, 1), 3);
    EXPECT_EQ(B->Get(1, 0), 2);

    delete B;
}

TEST(MatrixTest, FillMethodsWork) {
    SquareMatrix<int> A(3);
    A.Fill(7);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(A.Get(i, j), 7);
        }
    }

    A.FillIdentity();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(A.Get(i, j), (i == j ? 1 : 0));
        }
    }

    A.Fill(5);
    A.FillDiagonal(10);
    EXPECT_EQ(A.Get(0, 0), 10);
    EXPECT_EQ(A.Get(1, 1), 10);
    EXPECT_EQ(A.Get(2, 2), 10);
    EXPECT_EQ(A.Get(0, 1), 5);
}

TEST(MatrixTest, MapWhereReduceWork) {
    SquareMatrix<int> A(2);
    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    auto mapped = A.Map<int>([](int x) { return x * 2; });
    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(mapped->Get(0, 0), 2);
    EXPECT_EQ(mapped->Get(1, 1), 8);

    auto filtered = A.Where([](int x) { return x % 2 == 0; });
    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->Get(0, 0), 0);
    EXPECT_EQ(filtered->Get(0, 1), 2);
    EXPECT_EQ(filtered->Get(1, 0), 0);
    EXPECT_EQ(filtered->Get(1, 1), 4);

    int sum = A.Reduce<int>(0, [](int acc, int x) { return acc + x; });
    int product = A.Reduce<int>(1, [](int acc, int x) { return acc * x; });

    EXPECT_EQ(sum, 10);
    EXPECT_EQ(product, 24);

    delete mapped;
    delete filtered;
}

TEST(MatrixTest, CompoundAdditionWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    B(0, 0) = 5; B(0, 1) = 6;
    B(1, 0) = 7; B(1, 1) = 8;

    A += B;

    EXPECT_EQ(A.Get(0, 0), 6);
    EXPECT_EQ(A.Get(0, 1), 8);
    EXPECT_EQ(A.Get(1, 0), 10);
    EXPECT_EQ(A.Get(1, 1), 12);
}

TEST(MatrixTest, CompoundSubtractionWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 10; A(0, 1) = 20;
    A(1, 0) = 30; A(1, 1) = 40;

    B(0, 0) = 1; B(0, 1) = 2;
    B(1, 0) = 3; B(1, 1) = 4;

    A -= B;

    EXPECT_EQ(A.Get(0, 0), 9);
    EXPECT_EQ(A.Get(0, 1), 18);
    EXPECT_EQ(A.Get(1, 0), 27);
    EXPECT_EQ(A.Get(1, 1), 36);
}

TEST(MatrixTest, CompoundScalarMultiplicationWorks) {
    SquareMatrix<int> A(2);

    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    A *= 2;

    EXPECT_EQ(A.Get(0, 0), 2);
    EXPECT_EQ(A.Get(0, 1), 4);
    EXPECT_EQ(A.Get(1, 0), 6);
    EXPECT_EQ(A.Get(1, 1), 8);
}

TEST(MatrixTest, CompoundMatrixMultiplicationWorks) {
    SquareMatrix<int> A(2), B(2);

    A(0, 0) = 1; A(0, 1) = 2;
    A(1, 0) = 3; A(1, 1) = 4;

    B(0, 0) = 2; B(0, 1) = 0;
    B(1, 0) = 1; B(1, 1) = 2;

    A *= B;

    EXPECT_EQ(A.Get(0, 0), 4);
    EXPECT_EQ(A.Get(0, 1), 4);
    EXPECT_EQ(A.Get(1, 0), 10);
    EXPECT_EQ(A.Get(1, 1), 8);
}

TEST(MatrixTest, CompoundAdditionThrowsForDifferentSizes) {
    SquareMatrix<int> A(2), B(3);
    EXPECT_THROW(A += B, DifferentLengthException);
}

TEST(MatrixTest, CompoundMatrixMultiplicationThrowsForDifferentSizes) {
    SquareMatrix<int> A(2), B(3);
    EXPECT_THROW(A *= B, DifferentLengthException);
}
