import os, sys, inspect
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir) 

import matext as mx
import numpy as np

nparray = np.array([[1,3,6],[3,9,4]],dtype=np.float32)


nparray2 = np.array([[2,7,5],[2,-3,3]],dtype=np.float32)
mat1 = mx.Matrix(nparray)
mat2 = mx.Matrix(nparray2)

print('\nnparray:')
print(nparray)
print('\nnparray2:')
print(nparray2)

print('\nmat1:')
print(mat1.toNumpy())
print('\nmodified mat1:')
mat1.setElement(0, 4.5)
print(mat1.toNumpy())

print('\nnparray')
print(nparray)

print('\nmat2:')
print(mat2.toNumpy())

mat3 = mat1+mat1
print('\nmat3:')
print(mat3.toNumpy())