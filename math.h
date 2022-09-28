#ifndef QT_GO_MATH_H
#define QT_GO_MATH_H

#include <vector>
#include <iostream>
#include <assert.h>
#include <memory>

template<class T>
class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, int max);
    Matrix(const Matrix& other);

    int nRows() const { return rows; }
    int nCols() const { return cols; }
    int size() const { return rows * cols; }
    std::vector<std::pair<int, int>> adjacentNonzero(int row, int col);

    Matrix<T> operator+(Matrix<T>& other);
    Matrix<T> operator*(Matrix<T>& other);
    template<class U> friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);
    T& operator()(const int row, const int col);

    void clear() { data.clear(); }
    T& get(int row, int col) { return data[(row * nRows()) + col]; }
    T& front() { return data[0]; }
    T& back() { return data[nCols() * nRows()]; }

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
    struct Col
    {
        Matrix& parent;
        int col;
        Col(Matrix& parent, int col);
        T& operator[](int row) { return parent(col, row); }
        friend std::ostream& operator<<(std::ostream& os, const Col& nCol)
        {
            for (int i = 0; i < nCol.parent.nRows(); i++)
            {
                os << nCol.parent.data[(nCol.row * nCol.parent.nRows()) + i] << " ";
            }
            os << '\n';
            return os;
        }
        std::vector<T*> colvec;
    };
    Row row(int row) { return Row(*this, row); }
    Col col(int col) { return Col(*this, col); }


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

template<class T>
Matrix<T>::Col::Col(Matrix& parent, int col) : parent(parent), col(col)
{
    int rows = parent.nRows();
    colvec.resize(rows);

    for (int i = 0; i < rows; i++)
    {
        colvec[i] = &parent.data[(parent.nCols() * i) + col];
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

template<class T>
Matrix<T>::Matrix(int rows, int cols) : rows{rows}, cols{cols}, data{std::vector<T>(rows * cols)}
{
}

template<class T>
Matrix<T>::Matrix(const int rows, const int cols, int max) : rows{rows}, cols{cols}
{
    srand(time(0));
    data.resize(rows * cols);

    for (int i = 0; i < data.size(); i++)
    {
        data[i] = rand() % max;
    }
}

template<class T>
Matrix<T>::Matrix(const Matrix& other)
{
    data.resize(other.nRows() * other.nCols());

    for (int i = 0; i < data.size(); i++)
    {
        data[i] = other.data[i];
    }
}

template<class T>
T& Matrix<T>::operator()(const int row, const int col)
{
    assert(row >= 0 && row <= nRows());
    assert(col >= 0 && col <= nCols());

    return data[(row * nRows()) + col];
}

// For every adjacent point around coordinate [row][col], if the value is nonzero, add to the return vector
template<class T>
std::vector<std::pair<int, int>> Matrix<T>::adjacentNonzero(int row, int col) {

    std::vector<std::pair<int, int>> adj;

    for (auto &i: modifiers) {
        int adjx = row + i.first;
        int adjy = col + i.second;

        if ((adjx >= 0 && adjx <= nCols()) &&
            (adjy >= 0 && adjy <= nRows()))
        {
            if (data[(adjy * nRows()) + adjx] != 0)
                adj.push_back(i);
        }
    }
    return adj;
}

template<class T>
Matrix<T> Matrix<T>::operator+(Matrix<T>& other)
{
    assert(nRows() == other.nRows() && nCols() == other.nCols());

    Matrix<T> res(other.nRows(), other.nCols());
    for (int i = 0; i < data.size(); i++)
    {
        res.data[i] = data[i] + other.data[i];
    }
    return res;
}

template<class T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& other)
{
    assert(nCols() == other.nRows());

    Matrix<T> res(nRows(), other.nCols());
    res.data.resize(nRows() * other.nCols());

    for (int masterRow = 0; masterRow < nRows(); masterRow++)
    {
        for (int masterCol = 0; masterCol < other.nCols(); masterCol++)
        {
            int sum = 0;

            for (int element = 0; element < other.nRows(); element++)
            {
                auto mat0 = data[(nCols() * masterRow) + element];
                auto mat1 = other.data[(element * other.nCols()) + masterCol];

                sum = sum + (mat0 * mat1);
            }
            res.data[(masterRow * other.nCols()) + masterCol] = sum;
        }
    }

    std::cout << std::endl;
    return res;
}

template class Matrix<int>;

#endif //QT_GO_MATH_H
