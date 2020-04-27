#include "matrix.h"
#include "vector.h"

namespace py = pybind11;

namespace matext
{

Matrix::Matrix(size_t rows, size_t cols)
{
    m_rows = rows;
    m_cols = cols;
    m_size = m_rows * m_cols;
    
    //Allocate array for new matrix
    m_data = new float[m_size];
}

// TODO Handle conversion conversion if numpy array is not float32
Matrix::Matrix(py::array_t<float> np_input)
{
    py::buffer_info buf = np_input.request();
    m_rows = buf.shape[0];
    m_cols = buf.shape[1];
    m_size = buf.size;
    m_data = (float *) buf.ptr;
}

py::array_t<float>  Matrix::toNumpy()
{
    py::array_t<float> np_matrix = py::array_t<float>(std::vector<ptrdiff_t>{(int) m_rows, (int) m_cols}, m_data);
    return np_matrix;
}

Matrix operator +(const Matrix& a, const Matrix& b)
{
    if(a.m_rows != b.m_rows || a.m_rows != b.m_rows)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Matrix result(a.m_rows, a.m_cols);
    
    for (size_t idx = 0; idx < a.m_size; idx++)
    {
        result.m_data[idx] = a.m_data[idx] + b.m_data[idx];
    }

    return result;
}

Matrix operator -(const Matrix& a, const Matrix& b)
{
    if(a.m_rows != b.m_rows || a.m_rows != b.m_rows)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Matrix result(a.m_rows, a.m_cols);
    
    for (size_t idx = 0; idx < a.m_size; idx++)
    {
        result.m_data[idx] = a.m_data[idx] - b.m_data[idx];
    }

    return result;
}

Matrix operator *(const Matrix& a, const float& value)
{
    Matrix result(a.m_rows, a.m_cols);
    
    for (size_t idx = 0; idx < a.m_size; idx++)
    {
        result.m_data[idx] = a.m_data[idx] * value;
    }

    return result;
}

Matrix operator *(const float& value, const Matrix& a)
{
    return a*value;
}

Vector Matrix::operator*(const Vector& vec)
{
    Vector result(m_rows);

    for(int r = 0; r < m_rows; r++)
    {
        result.m_data[r] = 0;
        for (int c = 0; c < m_cols; c++)
        {
            result.m_data[r] += m_data[r*m_cols + c] * vec.m_data[c];
        }
    }
    return result;
}
} //end of matext namespace