#include "matrix.h"
#include "vector.h"

namespace py = pybind11;

namespace matext
{
PYBIND11_MODULE(matext, m) {
        py::class_<Matrix>(m, "Matrix")
        .def(py::init<py::array_t<float>>())
        .def("toNumpy", &Matrix::toNumpy, R"pbdoc(Return numpy array.)pbdoc")
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self / float())
        .def(py::self * float())
        .def(float()  * py::self)
        .def(py::self * py::self)
        .def("__mul__", py::overload_cast<const Vector&>(&Matrix::operator*, py::const_))
        .def("det", &Matrix::det)
        .def("submat", &Matrix::submat)
        .def("T", &Matrix::T)
        .def("inv", &Matrix::inv)
        .def("pinv", &Matrix::pinv);

        
        py::class_<Vector>(m, "Vector")
        .def(py::init<py::array_t<float>>())
        .def("toNumpy", &Vector::toNumpy, R"pbdoc(Return numpy array.)pbdoc")
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self / float())
        .def(py::self * float())
        .def(float()  * py::self);


        m.doc() = "Matrix class c++ extension for fast matrix calculations"; // optional module docstring
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
} //end of matext namespace