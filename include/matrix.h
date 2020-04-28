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
    
    Matrix(py::array_t<float, py::array::c_style | py::array::forcecast> np_input);
    Matrix(size_t rows, size_t cols);
    py::array_t<float> toNumpy();
    
    void setElement(int idx, float value) { m_data[idx] = value; }
    
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator/(const float& value) const;
    Matrix operator*(const float& value) const;
    Vector operator*(const Vector& vec) const;
    Matrix operator*(const Matrix& other) const;

    friend Matrix operator*(const float& value, const Matrix& mat);
    
    float det() const;
    Matrix submat(int row_ex, int col_ex) const;
    Matrix T() const;
    Matrix inv() const;
    Matrix pinv() const;

private:
    py::array_t<float, py::array::c_style> m_np;
    size_t m_dim;
    size_t m_rows, m_cols;
    size_t m_size;
    float *m_data;
};

} //end of matext namespace