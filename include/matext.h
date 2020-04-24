#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

//TODO Vector class
//TODO V.toNumpy()
//TODO -(Vec, Vec)
//TODO +(Vec, Vec)
//TODO *(Vec,Scal)
//TODO /(Vec,Scal)

namespace py = pybind11;
namespace matext
{
class Matrix
{
public:
    Matrix(size_t rows, size_t cols);
    Matrix(py::array_t<float> np_input);

    py::array_t<float> toNumpy();

    float* data() { return m_data; }
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    
    void setElement(int idx, float value) { m_data[idx] = value; }
    
    Matrix operator+(Matrix other) const;
    //TODO Matrix operator-(Matrix other) const;
    //TODO /(Mat,Scal)
    //TODO *(Mat,Scal)
    //TODO *(Vec,Mat)
    //TODO *(Mat,Mat)
    //TODO /(Mat,Mat)
    //TODO inv(M)
    //TODO pnv(M)

private:
    py::array_t<float> m_array;
    size_t m_rows, m_cols;
    size_t m_size;
    float *m_data;
};

} //end of matext namespace