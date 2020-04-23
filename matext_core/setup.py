import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

cpp_args = ['-std=c++11', '-stdlib=libc++', '-mmacosx-version-min=10.7']

matext_module = Extension(
    'matext_core', sources = ['core.cpp'],
    include_dirs=['pybind11/include'],
    language='c++',
    extra_compile_args = cpp_args,
    )

setup(
    name = 'matext_core',
    version = '1.0',
    description = 'Python package with vector and matrix calculation C++ extension (PyBind11)',
    ext_modules = [matext_module],
)