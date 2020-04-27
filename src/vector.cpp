#include "vector.h"

namespace py = pybind11;

namespace matext
{
Vector::Vector(size_t size)
{
    m_size = size;
    
    //Allocate array for new vector
    m_data = new float[m_size];
}

Vector::Vector(py::array_t<float> np_input)
{
    py::buffer_info buf = np_input.request();
    m_size = buf.size;
    m_data = (float *) buf.ptr;
}

py::array_t<float>  Vector::toNumpy()
{
    py::array_t<float> np_vector = py::array_t<float>(std::vector<ptrdiff_t>{(int) m_size}, m_data);
    return np_vector;
}


Vector operator +(const Vector& a, const Vector& b)
{
    if(a.m_size != b.m_size)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Vector result(a.m_size);
    
    for (size_t idx = 0; idx < a.m_size; idx++)
    {
        result.m_data[idx] = a.m_data[idx] + b.m_data[idx];
    }

    return result;
}
} //end of matext namespace