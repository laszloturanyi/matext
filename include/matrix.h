#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

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
        
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator/(const float& value) const;
    Matrix operator*(const float& value) const;
    Matrix operator*(const Matrix& other) const;
    Vector operator*(const Vector& vec) const;

    friend Matrix operator*(const float& value, const Matrix& mat);
    
    float det() const;
    Matrix T() const;
    Matrix inv() const;
    
    /**
     * Fast Computation of Moore-Penrose Inverse Matrices
     * Courrieu, P. (2008).
     * http://arxiv.org/abs/0804.4809
     */
    Matrix pinv() const;

private:
    Matrix submat(int row_ex, int col_ex) const;

private:
    py::array_t<float, py::array::c_style> m_np;
    size_t m_rows, m_cols;
    size_t m_size;
    float *m_data;
};

} //end of matext namespace