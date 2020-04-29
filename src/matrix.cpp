#include <math.h>

#include "matrix.h"
#include "vector.h"

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

    m_rows = buf.shape[0];
    m_cols = buf.shape[1];
    m_size = buf.size;
    m_data = (float *) buf.ptr;
} 

Matrix::Matrix(size_t rows, size_t cols)
{
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
    if(this->m_rows != other.m_rows || this->m_cols != other.m_cols)
    {
        throw std::runtime_error("Input dimensions must match");
    }
    Matrix result(this->m_rows, this->m_cols);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] + other.m_data[idx];
    }

    return result;
}

Matrix Matrix::operator -(const Matrix& other) const
{
    if(this->m_rows != other.m_rows || this->m_cols != other.m_cols)
    {
        throw std::runtime_error("Input dimensions must match");
    }
    Matrix result(this->m_rows, this->m_cols);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] - other.m_data[idx];
    }

    return result;
}

Matrix Matrix::operator /(const float& value) const
{
    Matrix result(this->m_rows, this->m_cols);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = this->m_data[idx] / value;
    }

    return result;
}

Matrix Matrix::operator *(const float& value) const
{
    Matrix result(this->m_rows, this->m_cols);
    
    for (size_t idx = 0; idx < this->m_size; idx++)
    {
        result.m_data[idx] = m_data[idx] * value;
    }

    return result;
}

Matrix operator *(const float& value, const Matrix& mat)
{
    return mat*value;
}

Matrix Matrix::operator*(const Matrix& other) const
{
    if (this->m_cols != other.m_rows)
    {
        throw std::runtime_error("Input dimensions must match");
    }

    Matrix result(this->m_rows, other.m_cols);

    for (int r1 = 0; r1 < this->m_rows; r1++)
    {
        for (int c2 = 0; c2 < other.m_cols; c2++)
        {
            result.m_data[r1*other.m_cols+c2] = 0;
            for (int c1 = 0; c1 < m_cols; c1++)
            {
                result.m_data[r1*other.m_cols + c2] += this->m_data[r1*this->m_cols + c1] * other.m_data[c1*other.m_cols + c2];
            }
        }
    }
    return result;
}

Vector Matrix::operator*(const Vector& vec) const
{
    if(this->m_cols != vec.m_size)
    {
        throw std::runtime_error("Input dimensions must match");
    }

    Vector result(this->m_rows);

    for(int r = 0; r < this->m_rows; r++)
    {
        result.m_data[r] = 0;
        for (int c = 0; c < this->m_cols; c++)
        {
            result.m_data[r] += this->m_data[r*m_cols + c] * vec.m_data[c];
        }
    }
    return result;
}

float Matrix::det() const
{
    if(this->m_rows != this->m_cols)
    {
        throw std::runtime_error("det() can be applied only to square matrices.");
    }

    size_t n = this->m_rows;

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
            det += sign * m_data[c] * this->submat(0,c).det();
            //Use alternating sign
            sign *= -1.0F;
        }
        return det;
    }
}

Matrix Matrix::T() const
{
    Matrix result(this->m_cols, this->m_rows);

    for (int r = 0; r < this->m_rows; r++)
    {
        for (int c = 0; c < this->m_cols; c++)
        {
            result.m_data[c*result.m_cols+r] = m_data[r*this->m_cols+c];
        }
    }

    return result;
}

Matrix Matrix::submat(int row_ex, int col_ex) const
{
    if(this->m_rows < 2 || this->m_cols < 2)
    {
        throw std::runtime_error("submat() cannot be applied to matrices where either dimension is lower than 2.");
    }
    if(row_ex >this->m_rows - 1)
    {
        throw std::runtime_error("Row index is out of range.");
    }
    if(col_ex > this->m_cols - 1)
    {
        throw std::runtime_error("Column index is out of range.");
    }

    Matrix submat = Matrix(this->m_rows-1 , this->m_cols-1);
    
    int idx = 0;
    for(int r = 0; r < this->m_rows; r++)
    {
        for(int c = 0; c < this->m_cols; c++)
        {
            if (r != row_ex && c != col_ex)
            {
                submat.m_data[idx] = this->m_data[r*this->m_cols+c];
                idx++;
            }
        }
    }

    return submat;
}

Matrix Matrix::inv() const
{
    if(this->m_rows != this->m_cols)
    {
        throw std::runtime_error("inv() can be applied only to square matrices.");
    }
    size_t n = this->m_rows;

    float determinant = this->det();
    if(determinant == 0)
    {
        throw std::runtime_error("Matrix is singular. Try pinv() instead.");
    }

    Matrix cofactor = Matrix(n, n);
    for(int r = 0; r < n; r++)
    {
        for(int c = 0; c < n; c++)
        {
            cofactor.m_data[r*n + c] = pow(-1.0F, r+c) * this->submat(r,c).det();
        }
    }

    return cofactor.T() / determinant;

}

Matrix Matrix::pinv() const
{
    // Transpose if m < n 
    bool transpose = false;

    size_t m = this->m_rows;
    size_t n = this->m_cols;
    
    //TODO this is not so elegant
    Matrix A = Matrix(n,m);

    if(m < n)
    {
        transpose = true;
        A = (*this) * this->T();
        n = m;
    }
    else
    {
        A = this->T() * (*this); 
    }

    // Full rank Cholesky factorization of A 
    
    // Get the lowest of positive diagonal elements for tolerance
    float tol = A.m_data[0];

    for (size_t idx = 0; idx <pow(n,2); idx += n+1)
    {
        //If tol was initialized as negative value, change to positive element
        if(A.m_data[idx] > 0 && (A.m_data[idx] < tol || tol < 0))
        {
            tol = A.m_data[idx];
        }
    }
    tol *= 1e-9F;
    
    Matrix L = Matrix(n,n);
    memset(L.m_data, 0, L.m_size * sizeof(float));

    // -1 instead of 0 because of zero based indexing. Later rank will be corrected.
    int r = -1;
    for (int k = 0; k < n ; k++)
    {
        r++;
        for (int row = k; row < n; row++)
        {
            L.m_data[row*n + r] = A.m_data[row*n + k];
            // If r = 0 subtracted vector is zero
            for (int  col = 0; col < r; col++)
            {
                L.m_data[row*n + r] -= L.m_data[row*n + col] * L.m_data[k*n + col];
            }
        }
        if (L.m_data[k*n + r] > tol)
        {
            L.m_data[k*n + r] = sqrt(L.m_data[k*n + r]);
            if (k < n-1)
            {
                for(int row = k+1; row < n; row++)
                {
                    L.m_data[row*n + r] /=  L.m_data[k*n + r];
                }
            }
        }
        else
        {
            r--;
        }
    }

    //Correcting rank because of zero based indexing
    r++;
    
    Matrix L2 = Matrix(n,r);
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < r; col++)
        {
            L2.m_data[row*r + col] = L.m_data[row*n + col];
        }
    }
    
    //Computation of the generalized inverse
    Matrix M = (L2.T()*L2).inv();
    if (transpose)
    {
        return this->T() * L2 * M * M * L2.T();
    }
    else
    {
        return L2 * M * M * L2.T() * this->T();
    }
}

} //end of matext namespace