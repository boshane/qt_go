#ifndef QT_GO_MATH_H
#define QT_GO_MATH_H

#include <vector>
#include <iostream>
#include <assert.h>

template<class T>
class Matrix
{
public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, int max);
    Matrix(const Matrix& other);

    int nRows() const { return rows; }
    int nCols() const { return cols; }
    std::vector<std::pair<int, int>> adjacentNonzero(int row, int col);

    Matrix<T> operator+(Matrix<T>& other);
    Matrix<T> operator*(Matrix<T>& other);
    template<class U> friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);
    T& operator()(const int row, const int col);

    struct Row
    {
        Matrix& parent;
        int row;
        Row(Matrix& parent, int row);
        T& operator[](int col) { return parent(row, col); }
        friend std::ostream& operator<<(std::ostream& os, const Row& nRow)
        {
            for (int i = 0; i < nRow.parent.nCols(); i++)
            {
                os << nRow.parent.data[(nRow.row * nRow.parent.nRows()) + i] << " ";
            }
            os << '\n';
            return os;
        }
        std::vector<T*> rowvec;
    };
    Row operator[](int row) { return Row(*this, row); }

    std::vector<T> data;
private:
    int rows, cols;
};

template<class T>
Matrix<T>::Row::Row(Matrix& parent, int row) : parent(parent), row(row)
{
    int cols = parent.nCols();
    rowvec.resize(cols);

    for (int i = 0; i < cols; i++)
    {
        rowvec[i] = &parent.data[(parent.nRows() * row) + i];
    }
}

template<class U>
std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix)
{
    for (int i = 0; i < matrix.nRows(); i++)
    {
        for (int j = 0; j < matrix.nCols(); j++)
        {
            os << matrix.data[(i * matrix.nCols()) + j] << " ";
        }
        os << '\n';
    }
    return os;
}

#endif //QT_GO_MATH_H
