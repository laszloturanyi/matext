#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

namespace py = pybind11;
namespace matext
{

class Vector;

class Matrix
{
public:
    Matrix(size_t rows, size_t cols);
    Matrix(py::array_t<float> np_input);

    py::array_t<float> toNumpy();
    
    void setElement(int idx, float value) { m_data[idx] = value; }
    
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    friend Matrix operator-(const Matrix& a, const Matrix& b);
    friend Matrix operator*(const Matrix& a, const float& value);
    friend Matrix operator*(const float& value, const Matrix& a);
    
    Vector operator*(const Vector& vec);
    
    //TODO /(Mat,Scal)
    //TODO *(Mat,Mat)
    //TODO /(Mat,Mat)
    //TODO inv(M)
    //TODO pnv(M)

private:
    size_t m_rows, m_cols;
    size_t m_size;
    float *m_data;
};

} //end of matext namespace