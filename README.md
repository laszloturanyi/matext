# matext
C++ Extension for Python implementing Vector and Matrix classes
The library implements Vector and Matrix classes with the following features:
- Matrix and Vector classes with underlying  ```float32``` arrays.
- Elementwise addition and subtraction.
- Elementwise multiplication and division by scalar number.
- Regular matrix and vector multiplications (M\*V and M\*M).
- Matrix inverse calculation based on [LU decomposition with partial pivoting](https://en.wikipedia.org/wiki/LU_decomposition#LU_factorization_with_partial_pivoting).
- [Fast Computation of Moore-Penrose Inverse Matrices](https://arxiv.org/abs/0804.4809)

## Prerequisites
These prerequisites are automatically installed when installing library from pip wheel.
- Python (tested with Python 3.6.8 x64)
- pip
- [wheel](https://pythonwheels.com/)
- [automatically installed] pybind11
- [automatically installed] numpy (required to initiate Matrices and Vectors)

## Install from pip wheel
Clone the repository and cd into the ```matext/dist```folder.  
The matext library can be installed from wheel file with following command executed from the ```dist``` directory:
```
pip install matext-0.1-cp36-cp36m-win_amd64.whl
```

## Build from source
Build  ```.pyd```dynamic library with the following command executed from the project root directory:

```
python setup.py build_ext --inplace
```

For the build process ```msvc``` is foced to used hence Visual Studio and MSBuild is assumed to be installated and MSBuild is assumed to be in the PATH).

The build process was tested with MSVC++ 14.25. Though other versions may be used too, be aware that ```overload_cast``` syntax in the binding code in ```src/main.cpp``` is a C++14-only feature so trying older version than MSVC++14.00 will result in problem.

Produce new wheel distribution file with the following command executed from the project root directory
```
python setup.py bdist_wheel
```

## How to use
```python
import matext as mx
import numpy as np

# Create numpy arrays
# float32 recommended to reduce conversion and copy overhead
npmat = np.array([[1,3,6],[3,9,4]],dtype=np.float32)
npvec = np.array([7,8,9],dtype=np.float32)

# Create matrix and vector objects
mat = mx.Matrix(npmat)
vec = mx.Vector(npvec)

# Multiply them and print results
vec_result = mat * vec
print(vec_result.toNumpy())
```

## Some design considerations...
- [Pybind11](https://github.com/pybind/pybind11) is used to create extension module becaus of its simplicity.
- Because both Marix and Vector objects should be created with numpy arrays ```py::array_t<float>``` type is used instead of raw buffer type. (see further notes in [Pybind11 doc](https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html#arrays)
- ```py::array::c_style``` ensures that arrays are stored in a dense manner and c-stlye (row major) ordering is used.
- ```<py::array::forcecast>``` ensures that non-conforming arguments are converted into an array satisfying the specified requirements.
- This way, numpy arrays storing ```float32``` data will not be copied but will be directly referenced by the Matrix and Vector objects. Other types will induce conversion, first.
- C160-C170 guidelines from [ISOC++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ro-conventional) were taken into considerations except [C163](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c161-use-non-member-functions-for-symmetric-operators) beause of how Pybind11 works.


## Performance
Run ```tests/benchmark.py``` to benchmark speed and accurary by comparing results with numpy.

The following runtimes were measured when executing operations with 400x400 matrices.

|Operation                  |matext       |Numpy        |
|:--------------            |------------:|------------:|
|Addition                   |0.000 sec    | 0.001 sec   |
|Multiplication             |0.181 sec    | 0.000 sec   |
|Transpose                  |0.001 sec    | 0.000 sec   |
|Inverse                    |0.217 sec    | 0.013 sec   |
|Pseudoinverse  (400x600)   |1.486 sec    | 0.098 sec   |

CPU                        |OS         |Compiler  |
:--------------------------|:----------|:---------|
[Intel® Core™ i7-7700HQ @ 2.8 GHz]      |Windows 10 x64 (18363)  |MSVC++ 14.25|