import matext.core.build
import matext.core.core as core
import numpy as np

a = np.zeros((10,3))
b = np.ones((10,3)) * 3 
c = core.add_arrays(a, b)
print(c)
