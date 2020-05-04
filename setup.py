#import os, sys

#from distutils.core import setup, Extension
#from distutils import sysconfig

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
matext_module = Extension(
    'matext', sources = ['src/matrix.cpp', 'src/vector.cpp', 'src/main.cpp'],
    include_dirs=['pybind11/include', 'include'],
    language='c++',
    )

setup(
    name = 'matext',
    version = '0.1',
    description = 'C++ Extension for Python implementing Vector and Matrix classes',
    url='https://github.com/laszloturanyi/matext',
    author='Laszlo Turanyi',
    author_email='turanyilaszlo95@gmail.com',
    license='MIT',
    setup_requires=['pybind11>=2.2'], 
    install_requires=['pybind11>=2.2',
                      'numpy>=1.18.3',
                      ],
    ext_modules = [matext_module],
)