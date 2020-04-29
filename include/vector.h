#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

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
    Vector(py::array_t<float, py::array::c_style | py::array::forcecast> np_input);
    Vector(size_t size);

    py::array_t<float> toNumpy();

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator/(const float& value) const;
    Vector operator*(const float& value) const;

    friend Vector operator*(const float& value, const Vector& vec);
    friend Vector Matrix::operator*(const Vector& vec) const;

private:
    py::array_t<float, py::array::c_style> m_np;
    size_t m_size;
    float *m_data;
    
};


} //end of matext namespace