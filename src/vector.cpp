#include "vector.h"

namespace py = pybind11;

namespace matext
{

Vector::Vector(py::array_t<float, py::array::c_style | py::array::forcecast> np_input)
: m_np(np_input)
{
    py::buffer_info buf = m_np.request();

    if( buf.ndim != 1)
    {
        throw std::runtime_error("Cannot create Vector object.\nInput should be 1-D numpy array");
    }

    m_size = buf.size;
    m_data = (float *) buf.ptr;
}

Vector::Vector(size_t size)
{
    m_size = size;
    
    //Allocate array for new vector
    m_np = py::array_t<float>(std::vector<ptrdiff_t>{(int) m_size});
    py::buffer_info buf = m_np.request();

    m_data = (float *) buf.ptr;
}



py::array_t<float>  Vector::toNumpy()
{
    return m_np;
}

Vector Vector::operator+(const Vector& other) const
{
    if(this->m_size != other.m_size)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Vector result(this->m_size);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] + other.m_data[idx];
    }

    return result;
}

Vector Vector::operator-(const Vector& other) const
{
    if(this->m_size != other.m_size)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Vector result(this->m_size);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] - other.m_data[idx];
    }

    return result;
}

Vector Vector::operator/(const float& value) const
{
    Vector result(this->m_size);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] / value;
    }

    return result;
}

Vector Vector::operator*(const float& value) const
{
    Vector result(this->m_size);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] * value;
    }

    return result;
}

Vector operator*(const float& value, const Vector& vec)
{
    return vec*value;
}
} //end of matext namespace