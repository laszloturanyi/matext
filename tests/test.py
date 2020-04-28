import os, sys, inspect
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir) 

import matext as mx
import numpy as np

# Create numpy arrays
npmat = np.array([[1,3,6],[3,9,4]],dtype=np.float32)
npmat2 = np.array([[2,7,5],[2,-3,3]],dtype=np.float32)

npvec = np.array([7,8,9],dtype=np.float32)
npvec2 = np.array([5,3,11],dtype=np.float32)

# Try to create invalid Matrix instances from 1D array
try:
    mx.Matrix(np.random.randn(4))
except RuntimeError as err:
    print(err)

# Try to create invalid Matrix instances from 3D array
try:
    mx.Matrix(np.random.randn(4,4,5))
except RuntimeError as err:
    print(err)

# Create valid matrix instances
mat = mx.Matrix(npmat)
mat2 = mx.Matrix(npmat2)

print('\nnpmat:')
print(npmat)
print('\nnpmat2:')
print(npmat2)

print('\nmat:')
print(mat.toNumpy())
print('\nmodified mat:')
mat.setElement(0, 4.5)
print(mat.toNumpy())

print('\nnpmat')
print(npmat)

print('\nmat2:')
print(mat2.toNumpy())


mat3 = mat + mat2
print('\nmat3:')
print(mat3.toNumpy())

mat4 = mat2 - mat
print('\nmat4:')
print(mat4.toNumpy())

 
mat5 =  mat2 *5
mat5 = 5 * mat2  
print('\nmat5:')
print(mat5.toNumpy())

npvec = np.array([7,8,9],dtype=np.float32)
npvec2 = np.array([5,3,11],dtype=np.float32)

vec = mx.Vector(npvec)
vec2 = mx.Vector(npvec2)

print('\nvec:')
print(vec.toNumpy())

print('\nvec2:')
print(vec2.toNumpy())

vec3 = vec + vec2
print('\nvec3:')
print(vec3.toNumpy())

vec4 = mat * vec
print('\nvec4:')
print(vec4.toNumpy())

print('mat^T:')
print(mat.T().toNumpy())



sq_array = np.array([[1,2,3,4],[5,2,9,7],[1,-1,3,-5],[7,8,1,6]])
sq_matrix =mx.Matrix(sq_array)
print('\nsq_matrix:')
print(sq_matrix.toNumpy())

print('\nsq_matrix submat:')
print(sq_matrix.submat(0,0).toNumpy())
print('\ndet:')
print(sq_matrix.det())
print(np.linalg.det(sq_array))

print('\ninv:')
print(sq_matrix.inv().toNumpy())

print('\nnumpy inv:')
np_inv = np.linalg.inv(sq_array)
print(np_inv)

print('\n check inv:')
id_mat = sq_matrix * sq_matrix.inv()
print(id_mat.toNumpy())

rect_array = np.array([[1,2,3,4],[5,2,9,7],[1,-1,3,-5]])
rect_matrix = mx.Matrix(rect_array)

print('\nnumpy pinv:')
print(np.linalg.pinv(rect_array))

print((rect_matrix*rect_matrix.T()).toNumpy())
print('\nmatext pinv:')
pinv_mat = rect_matrix.pinv()
M = (pinv_mat.T()*pinv_mat).inv()
print(pinv_mat.toNumpy())
#print(pinv_mat.toNumpy())