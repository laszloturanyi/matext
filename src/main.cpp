#include <pybind11/operators.h>

#include "matrix.h"
#include "vector.h"

namespace py = pybind11;

namespace matext
{
PYBIND11_MODULE(matext, m)
{
        py::class_<Matrix>(m, "Matrix")
        .def(py::init<py::array_t<float>>(), R"pbdoc(Creates a Matrix object initialized by a numpy array)pbdoc")
        .def("toNumpy", &Matrix::toNumpy, R"pbdoc(Returns matrix as numpy array)pbdoc")
        .def(py::self + py::self, R"pbdoc(Elementwise addition of two matrices)pbdoc")
        .def(py::self - py::self, R"pbdoc(Elementwise subtraction of two matrices)pbdoc")
        .def(py::self / float(), R"pbdoc(Divides all elements of a matrix with a scalar value)pbdoc")
        .def(py::self * float(), R"pbdoc(Multiplies all elements of a matrix with a scalar value)pbdoc")
        .def(float()  * py::self, R"pbdoc(Multiplies all elements of a matrix with a scalar value)pbdoc")
        .def(py::self * py::self, R"pbdoc(Returns matrix product of two matrices)pbdoc")
        .def("__mul__", py::overload_cast<const Vector&>(&Matrix::operator*, py::const_),
        R"pbdoc(Returns matrix-vector product of a matrix and a vector)pbdoc")
        .def("det", &Matrix::det, R"pbdoc(Returns the determinant of a square matrix)pbdoc")
        .def("T", &Matrix::T, R"pbdoc(Returns the transpose of a matrix)pbdoc")
        .def("inv", &Matrix::inv, R"pbdoc(Returns the inverse of a square matrix if nonsingular)pbdoc")
        .def("pinv", &Matrix::pinv, R"pbdoc(Calculates the Moore-Penrose pseudoinverse of a matrix based on a full rank Cholesky factorization)pbdoc");
        
        py::class_<Vector>(m, "Vector")
        .def(py::init<py::array_t<float>>(), R"pbdoc(Creates a Vector object initialized by a numpy array)pbdoc")
        .def("toNumpy", &Vector::toNumpy, R"pbdoc(Returns vector as numpy array)pbdoc")
        .def(py::self + py::self, R"pbdoc(Elementwise addition of two vectors)pbdoc")
        .def(py::self - py::self, R"pbdoc(Elementwise subtraction of two vectors)pbdoc")
        .def(py::self / float(), R"pbdoc(Divides all elements of a vector with a scalar value)pbdoc")
        .def(py::self * float(), R"pbdoc(Multiplies all elements of a vector with a scalar value)pbdoc")
        .def(float()  * py::self, R"pbdoc(Multiplies all elements of a vector with a scalar value)pbdoc");
}
} //end of matext namespace