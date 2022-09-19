#include "math.h"

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
    return data[(row * nRows()) + col];
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
