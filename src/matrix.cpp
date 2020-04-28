#include "matrix.h"
#include "vector.h"

# include <iostream>
namespace matext
{
Matrix::Matrix(py::array_t<float, py::array::c_style | py::array::forcecast> np_input)
: m_np(np_input)
{
    py::buffer_info buf = m_np.request();

    if( buf.ndim != 2)
    {
        throw std::runtime_error("Cannot create Matrix object.\nInput should be 2-D numpy array");
    }

    m_dim = buf.ndim;
    m_rows = buf.shape[0];
    m_cols = buf.shape[1];
    m_size = buf.size;
    m_data = (float *) buf.ptr;
} 

Matrix::Matrix(size_t rows, size_t cols)
{
    m_dim = size_t(2);
    m_rows = rows;
    m_cols = cols;
    m_size = m_rows * m_cols;

    //Allocate array for new matrix
    m_np = py::array_t<float>(std::vector<ptrdiff_t>{(int) m_rows, (int) m_cols});
    py::buffer_info buf = m_np.request();    

    m_data = (float *) buf.ptr;
}

py::array_t<float>  Matrix::toNumpy()
{
    return m_np;
}

Matrix Matrix::operator +(const Matrix& other) const
{
    if(m_rows != other.m_rows || m_cols != other.m_cols)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Matrix result(m_rows, m_cols);
    
    for (size_t idx = 0; idx < m_size; idx++)
    {
        result.m_data[idx] = m_data[idx] + other.m_data[idx];
    }

    return result;
}

Matrix Matrix::operator -(const Matrix& other) const
{
    if(m_rows != other.m_rows || m_cols != other.m_cols)
    {
        throw std::runtime_error("Input shapes must match");
    }
    Matrix result(m_rows, m_cols);
    
    for (size_t idx = 0; idx < m_size; idx++)
    {
        result.m_data[idx] = m_data[idx] - other.m_data[idx];
    }

    return result;
}

Matrix Matrix::operator /(const float& value) const
{
    Matrix result(m_rows, m_cols);
    
    for (size_t idx = 0; idx < m_size; idx++)
    {
        result.m_data[idx] = m_data[idx] / value;
    }

    return result;
}

Matrix Matrix::operator *(const float& value) const
{
    Matrix result(m_rows, m_cols);
    
    for (size_t idx = 0; idx < m_size; idx++)
    {
        result.m_data[idx] = m_data[idx] * value;
    }

    return result;
}

Matrix operator *(const float& value, const Matrix& mat)
{
    return mat*value;
}

Vector Matrix::operator*(const Vector& vec) const
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


Matrix Matrix::operator*(const Matrix& other) const
{
    if (m_cols != other.m_rows)
    {
        throw std::runtime_error("Input shapes must match");
    }

    Matrix result(m_rows, other.m_cols);

    for (int r1 = 0; r1 < m_rows; r1++)
    {
        for (int c2 = 0; c2 < other.m_cols; c2++)
        {
            result.m_data[r1*other.m_cols+c2] = 0;
            for (int c1 = 0; c1 < m_cols; c1++)
            {
                result.m_data[r1*other.m_cols + c2] += m_data[r1*m_cols + c1] * other.m_data[c1*other.m_cols + c2];
            }
        }
    }
    return result;
}




float Matrix::det() const
{
    if(m_rows != m_cols)
    {
        throw std::runtime_error("det() can be applied only to square matrices.");
    }

    size_t n = m_rows;

    if(n==1)
    {
        return m_data[0];
    }
    else if(n==2)
    {
        return m_data[0]*m_data[3] - m_data[1]*m_data[2];
    }

    else
    {
        float det = 0.0F;
        float sign = 1.0F;
        //Itarate along first row
        for (int c = 0; c < n; c++)
        {
            det += sign * m_data[c] * submat(0,c).det();
            //Use alternating sign
            sign *= -1.0F;
        }
        return det;
    }
}

Matrix Matrix::T() const
{
    Matrix result(m_cols, m_rows);

    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            result.m_data[c*result.m_cols+r] = m_data[r*m_cols+c];
        }
    }

    return result;
}

Matrix Matrix::submat(int row_ex, int col_ex) const
{
    if(m_rows < 2 || m_cols < 2)
    {
        throw std::runtime_error("minor() cannot be applied to matrices where either dimension is lower than 2.");
    }
    if(row_ex > m_rows - 1)
    {
        throw std::runtime_error("Row index is out of range.");
    }
    if(col_ex > m_cols - 1)
    {
        throw std::runtime_error("Column index is out of range.");
    }

    Matrix submat = Matrix(m_rows-1 , m_cols-1);
    
    int idx = 0;
    for(int r = 0; r < m_rows; r++)
    {
        for(int c = 0; c < m_cols; c++)
        {
            if (r != row_ex && c != col_ex)
            {
                submat.m_data[idx] = m_data[r*m_cols+c];
                idx++;
            }
        }
    }

    return submat;
}

Matrix Matrix::inv() const
{
    if(m_rows != m_cols)
    {
        throw std::runtime_error("inv() can be applied only to square matrices.");
    }
    size_t n = m_rows;

    float determinant = det();
    if(determinant == 0)
    {
        throw std::runtime_error("Matrix is singular. Try pinv() instead.");
    }

    Matrix cofactor = Matrix(n, n);
    for(int r = 0; r < n; r++)
    {
        for(int c = 0; c < n; c++)
        {
            cofactor.m_data[r*n + c] = pow(-1.0F, r+c) * submat(r,c).det();
        }
    }

    return cofactor.T() / determinant;

}

Matrix Matrix::pinv() const
{

}

} //end of matext namespace