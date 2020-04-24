#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>

#include <Windows.h>
#include <cmath>

#include <iostream>


namespace py = pybind11;

//TODO Vector class
//TODO V.toNumpy()

class Matrix {
public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        m_rows = rows;
        m_cols = cols;
        m_size = m_rows * m_cols;
        
        //Allocate array for new matrix
        m_data = new float[m_size];
    }
    // TODO Handle conversion conversion if numpy array is notfloat32
    Matrix(py::array_t<float> np_input)
    {
        py::buffer_info buf = np_input.request();

        m_rows = buf.shape[0];
        m_cols = buf.shape[1];
        m_size = buf.size;
        float* ptr = (float *) buf.ptr;
        m_data = (float *) buf.ptr;
    }
    float *data() { return m_data; }
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    void setElement(int idx, float value) {m_data[idx] = value;}

    // TODO This should receive const matrix as input
    // TODO maybe this should be symmetric
    Matrix operator +(Matrix other) const{
        if(other.rows() != m_rows || other.cols() != m_cols)
        {
            throw std::runtime_error("Input shapes must match");
        }
        
        Matrix result(m_rows, m_cols);
        float *other_data = other.data();
        float *result_data = result.data();
        for (size_t idx = 0; idx < m_size; idx++)
        {
            result_data[idx] = m_data[idx] + other_data[idx];
        }

        return result;

    }
    py::array_t<float>  toNumpy(){
        py::array_t<float> np_matrix = py::array_t<float>(std::vector<ptrdiff_t>{(int) m_rows, (int) m_cols}, m_data);
        return np_matrix;
    }
private:
    py::array_t<float> m_array;
    size_t m_rows, m_cols;
    size_t m_size;
    float *m_data;
};

//TODO -(Mat, Mat)
//TODO +(Mat, Mat)
//TODO -(Vec, Vec)
//TODO +(Vec, Vec)
//TODO *(Vec,Scal)
//TODO *(Mat,Scal)
//TODO /(Vec,Scal)
//TODO /(Mat,Scal)

//TODO *(Vec,Mat)
//TODO /(Mat,Mat)

//TODO inv(M)
//TODO pnv(M)


PYBIND11_MODULE(core, m) {
        py::class_<Matrix>(m, "Matrix")
        .def(py::init<py::array_t<float>>())
        .def("rows", &Matrix::rows)
        .def("cols", &Matrix::cols)
        .def("setElement", &Matrix::setElement)
        .def("toNumpy", &Matrix::toNumpy, R"pbdoc(Return numpy array.)pbdoc")
        .def(py::self + py::self, R"pbdoc(Return numpy array.)pbdoc");
        m.doc() = "Matrix class c++ extension for fast matrix calculations"; // optional module docstring


#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}