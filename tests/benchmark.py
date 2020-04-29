# -*- coding: utf-8 -*-

from context import matext as mx
import numpy as np
import time
import operator as op

def time_function(times, f, *args):
    tic = time.time()
    for i in range(times):
        f(*args)
    toc = time.time()
    return toc - tic
    
def print_result (operation, matext_time, numpy_time, difference):
    print('[{}]\t Matex runtime: {:.3f} sec,\
        Numpy runtime: {:.3f} sec,\
        the Frobenius norm based distance: {}'\
        .format(operation, matext_time, numpy_time, difference))


if __name__ == '__main__':

    # Let's compare how fast the implementations with numpy

    times = 1
    N = 10
    M = 14

    np_array1 = np.random.rand(N,N).astype(np.float32)
    mat1 = mx.Matrix(np_array1)

    np_array2 = np.random.rand(N,N).astype(np.float32)
    mat2 = mx.Matrix(np_array2)

    np_array3 = np.random.rand(N,M).astype(np.float32)
    mat3 = mx.Matrix(np_array3)


    # Benchmark matrix addition
    np_array_sum = np_array1 + np_array2
    mat_sum = mat1 + mat2
    difference = np.linalg.norm(mat_sum.toNumpy() - np_array_sum, ord='fro')
    
    matext_time = time_function(times, op.add, mat1, mat2)
    numpy_time = time_function(times, op.add, np_array1, np_array2)
    print_result("+", matext_time, numpy_time, difference)


    # Benchmark matrix multiplication
    np_array_prod = np_array1.dot(np_array2)
    mat_prod = mat1 * mat2
    difference = np.linalg.norm(mat_prod.toNumpy() - np_array_prod, ord='fro')

    matext_time = time_function(times, op.mul, mat1, mat2)
    numpy_time = time_function(times, op.mul, np_array1, np_array2)
    print_result("*", matext_time, numpy_time, difference)
    
    # Benchmark for determinant calculation
    np_array_det = np.linalg.det(np_array1)
    mat_det = mat1.det()
    difference = mat_det - np_array_det
    matext_time = time_function(times, mat1.det)
    numpy_time = time_function(times, np.linalg.det, np_array1)
    print_result("det", matext_time, numpy_time, difference)

    ## Benchmark for inverse calculation
    #np_array_inv = np.linalg.inv(np_array1)
    #mat_inv = mat1.inv()
    #difference = np.linalg.norm(mat_inv.toNumpy() - np_array_inv, ord='fro')
    #matext_time = time_function(times, mat1.inv)
    #numpy_time = time_function(times, np.linalg.inv, np_array1)
    #print_result("inv", matext_time, numpy_time, difference)
#
    ## Benchmark for pseudoinverse calculation
    #np_array_pinv = np.linalg.pinv(np_array1)
    #mat_pinv = mat1.pinv()
    #difference = np.linalg.norm(mat_pinv.toNumpy() - np_array_pinv, ord='fro')
    #matext_time = time_function(times, mat1.pinv)
    #numpy_time = time_function(times, np.linalg.pinv, np_array1)
    #print_result("pinv", matext_time, numpy_time, difference)