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
        Maximum elementwise difference: {}'\
        .format(operation, matext_time, numpy_time, difference))


if __name__ == '__main__':

    # Let's compare with numpy how fast the implementations are
    times = 1
    N = 400
    M = 600

    np_array1 = np.random.rand(N,N).astype(np.float32)
    mat1 = mx.Matrix(np_array1)

    np_array2 = np.random.rand(N,N).astype(np.float32)
    mat2 = mx.Matrix(np_array2)

    np_array3 = np.random.rand(N,M).astype(np.float32)
    mat3 = mx.Matrix(np_array3)

    # Benchmark matrix addition
    np_array_sum = np_array1 + np_array2
    mat_sum = mat1 + mat2
    difference = np.max(abs((mat_sum.toNumpy() - np_array_sum)))
    matext_time = time_function(times, op.add, mat1, mat2)
    numpy_time = time_function(times, op.add, np_array1, np_array2)
    print_result("+", matext_time, numpy_time, difference)

    # Benchmark matrix multiplication
    np_array_prod = np_array1.dot(np_array2)
    mat_prod = mat1 * mat2
    difference = np.max(abs((mat_prod.toNumpy() - np_array_prod)))
    matext_time = time_function(times, op.mul, mat1, mat2)
    numpy_time = time_function(times, op.mul, np_array1, np_array2)
    print_result("*", matext_time, numpy_time, difference)

    # Benchmark for transpose calculation
    np_array_T = np_array1.T
    mat_T = mat1.T()
    difference = np.max(abs((mat_T.toNumpy() - np_array_T)))
    matext_time = time_function(times, mat1.T)
    numpy_time = time_function(times, np.transpose, np_array1)
    print_result("T", matext_time, numpy_time, difference)

    #Benchmark for inverse calculation
    np_array_inv = np.linalg.inv(np_array1)
    mat_inv = mat1.inv()
    difference = np.max(abs((mat_inv.toNumpy() - np_array_inv)))
    matext_time = time_function(times, mat1.inv)
    numpy_time = time_function(times, np.linalg.inv, np_array1)
    print_result("inv", matext_time, numpy_time, difference)

    # Benchmark for pseudoinverse calculation
    np_array_pinv = np.linalg.pinv(np_array3)
    mat_pinv = mat3.pinv()
    difference = np.max(abs((mat_pinv.toNumpy() - np_array_pinv)))
    matext_time = time_function(times, mat1.pinv)
    numpy_time = time_function(times, np.linalg.pinv, np_array1)
    print_result("pinv", matext_time, numpy_time, difference)