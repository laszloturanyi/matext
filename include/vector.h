#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include "matrix.h"

//TODO Vector class
//TODO V.toNumpy()
//TODO -(Vec, Vec)
//TODO +(Vec, Vec)
//TODO *(Vec,Scal)
//TODO /(Vec,Scal)

namespace py = pybind11;
namespace matext
{


class Vector
{
public:
    Vector(size_t size);
    Vector(py::array_t<float> np_input);
    
    py::array_t<float> toNumpy();

    friend Vector operator+(const Vector& a, const Vector& b);
    friend Vector Matrix::operator*(const Vector& vec) const;

private:
    float *m_data;
    size_t m_size;
    
};


} //end of matext namespace