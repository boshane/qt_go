#include "math.h"
#include "game.h"

template<class T>
Matrix<T>::Matrix()
{
}

template<class T>
Matrix<T>::Matrix(int rows, int cols) : rows{rows}, cols{cols}
{
    data.resize(rows * cols, 0);
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
