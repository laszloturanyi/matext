#include <math.h>

#include "matrix.h"
#include "vector.h"

#include <iostream>
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
    auto other_T = other.T();
    Matrix result(this->m_rows, other.m_cols);
    
    memset(result.m_data, 0, result.m_size * sizeof(float));
    
    for (int r1 = 0; r1 < this->m_rows; r1++)
    {
        for (int c1 = 0; c1 < this->m_cols; c1++)
        {
            for (int c2 = 0; c2 < other.m_cols; c2++)
            {
                result(r1, c2) += (*this)(r1, c1) * other(c1, c2);
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
            result.m_data[r] += (*this)(r,c) * vec.m_data[c];
        }
    }
    return result;
}


Matrix Matrix::T() const
{
    Matrix result(this->m_cols, this->m_rows);

    for (int r = 0; r < this->m_rows; r++)
    {
        for (int c = 0; c < this->m_cols; c++)
        {
            result(c,r) = (*this)(r,c);
        }
    }

    return result;
}

float& Matrix::operator()(size_t row, size_t col)
{
    if(row > this->m_rows - 1)
    {
        throw std::runtime_error("Row index is out of range.");
    }
    if(col > this->m_cols - 1)
    {
        throw std::runtime_error("Column index is out of range.");
    }

    return this->m_data[row*this->m_cols + col];
}

float& Matrix::operator()(size_t row, size_t col) const
{
    if(row > this->m_rows - 1)
    {
        throw std::runtime_error("Row index is out of range.");
    }
    if(col > this->m_cols - 1)
    {
        throw std::runtime_error("Column index is out of range.");
    }

    return this->m_data[row*this->m_cols + col];
}

void Matrix::setIdentity()
{
    if(this->m_rows != this->m_cols)
    {
        throw std::runtime_error("setIdentity() can be applied only to square matrices.");
    }
    size_t N = this->m_rows; 
    
    memset(this->m_data, 0, this->m_size * sizeof(float));
    
    for (size_t idx = 0; idx <pow(N,2); idx += N+1)
    {
        this->m_data[idx] = 1;
    }
}

void Matrix::swapRows(size_t row1, size_t row2)
{
    if(row1 > this->m_rows - 1 || row2 > this->m_rows - 1)
    {
        throw std::runtime_error("Row index is out of range.");
    }

    size_t row_size = this->m_rows*sizeof(float);
    float* temp = new float[this->m_rows];
    
    float* row1_ptr = m_data + row1 * this->m_cols;
    float* row2_ptr = m_data + row2 * this->m_cols;

    memcpy(temp, row1_ptr, row_size);
    memcpy(row1_ptr, row2_ptr, row_size);
    memcpy(row2_ptr, temp, row_size);

    delete temp;
    
}

void Matrix::swapColumns(size_t col1, size_t col2)
{
    if(col1 > this->m_cols - 1 || col2 > this->m_cols - 1)
    {
        throw std::runtime_error("Column index is out of range.");
    }
    float temp = 0.0F;

    for (size_t r = 0; r < this->m_rows; r ++)
    {
        temp = (*this)(r, col1);
        (*this)(r, col1) = (*this)(r, col2);
        (*this)(r, col2) = temp;
    }
}



Matrix Matrix::inv() const
{
    
    if(this->m_rows != this->m_cols)
    {
        throw std::runtime_error("inv() can be applied only to square matrices.");
    }
    size_t N = this->m_rows;

    Matrix L = Matrix(N,N);
    L.setIdentity();

    Matrix U = Matrix(N,N);
    memcpy(U.m_data, this->m_data, (size_t) pow(N,2)*sizeof(float));

    // P, Y, X is stored in the same matrix
    Matrix P = Matrix(N,N);
    P.setIdentity();

    // Swap rows where diagonal element is zero but there is non zero
    // element in a row below.
    for(size_t k = 0; k < N; k++)
    {
        if(fabs(U(k,k)) < FLT_EPSILON)
        {
            for(size_t i = k+1; i < N; i++)
            {
                if(fabs(U(i,k)) > FLT_EPSILON)
                {
                    U.swapRows(i,k);
                    P.swapRows(i,k);
                    L.swapRows(i,k);
                    L.swapColumns(i,k);
                    break;
                }
            }
        }

        // If diagonal element is still zero, matrix is not invertable
        if(fabs(U(k,k)) < FLT_EPSILON)
        {
            throw std::runtime_error("Matrix is not invertable."); 
        }

        // For all rows below diagonal
        for(size_t i = k+1; i < N; i++)
        {
            // A(i,k) / A(k,k)
            // but A cannot be used because rows are swapped
            L(i,k) = U(i,k) / U(k,k);

            // add i-th row and n-th row
            // multiplied by: -A(i,k) / A(k,k)
            for (size_t l = k; l < N; l++) {
                U(i, l) -= L(i,k) * U(k,l);
            }
        }
    }
        
    // solve LY=P*I
    // Y is stored in the same matrix as P
    // for all columns of Y
    for (size_t c = 0; c < N; c++) {
        // for all rows of L
        for (size_t i = 0; i < N; i++) {
            // for all columns of L
            for (size_t j = 0; j < i; j++) {
                // for all existing y
                P(i,c) -= L(i,j) * P(j,c);
            }
        }
    }
    // solve Ux=y
    // for all columns of X
    for (size_t c = 0; c < N; c++) {
        // for all rows of U
        for (size_t l = 0; l < N; l++) {
            // have to go in reverse order
            size_t i = N - 1 - l;
            // for all columns of U
            for (size_t j = i + 1; j < N; j++) {
                // for all existing x
                P(i, c) -= U(i, j) * P(j, c);
            }
            // U(i,i) != 0
            P(i, c) /= U(i, i);
        }
    }
    
    //check if all P(i,j) is finite
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            if (!isfinite(P(i,j))) {
                throw std::runtime_error("Matrix is not invertable."); 
            }
        }
    }

    return P;
}

Matrix Matrix::pinv() const
{
    // Transpose if M < N 
    bool transpose = false;

    size_t M = this->m_rows;
    size_t N = this->m_cols;
    
    Matrix A = Matrix(M,N);

    if(M < N)
    {
        transpose = true;
        A = (*this) * this->T();
        N = M;
    }
    else
    {
        A = this->T() * (*this); 
    }

    // Full rank Cholesky factorization of A 
    
    // Get the lowest of positive diagonal elements for tolerance
    float tol = A.m_data[0];

    for (size_t i = 0; i < N; i++)
    {
        //If tol was initialized as negative value, change to positive element
        if(A(i,i) > 0 && (A(i,i) < tol || tol < 0))
        {
            tol = A(i,i);
        }
    }
    tol *= 1e-9F;
    
    Matrix L = Matrix(N,N);
    memset(L.m_data, 0, L.m_size * sizeof(float));

    // -1 instead of 0 because of zero based indexing. Later rank will be corrected.
    int r = -1;
    for (int k = 0; k < N ; k++)
    {
        r++;
        for (int row = k; row < N; row++)
        {
            L(row,r) = A(row,k);
            // If r = 0 subtracted vector is zero
            for (int  col = 0; col < r; col++)
            {
                L(row,r) -= L(row,col) * L(k,col);
            }
        }
        if (L(k,r) > tol)
        {
            L(k,r) = sqrt(L(k,r));
            if (k < N-1)
            {
                for(int row = k+1; row < N; row++)
                {
                    L(row,r) /=  L(k,r);
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
    
    Matrix L2 = Matrix(N,r);
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < r; col++)
        {
            L2(row,col) = L(row,col);
        }
    }
    
    //Computation of the pseudoinverse
    Matrix L2_inv = (L2.T()*L2).inv();
    if (transpose)
    {
        return this->T() * L2 * L2_inv * L2_inv * L2.T();
    }
    else
    {
        return L2 * L2_inv * L2_inv * L2.T() * this->T();
    }
}

} //end of matext namespace