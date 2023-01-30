from ctypes import *
import numpy as np

class array(Structure):
    _fields_ = [
	("cols", c_int),
	("rows", c_int),
	("data", POINTER(c_double))
]

class assignement_result(Structure):
    _fields_ = [
		("agent_to_object", POINTER(c_int)),
		("object_to_agent", POINTER(c_int)),
		("len", c_int)
	]

ptr_array = POINTER(array)
ptr_assig_res = POINTER(assignement_result)
cauction_lib = CDLL("/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/bin/cauction_lib.so")

np_array = np.array([[0.08411499, 0.92580858, 1.78757778,  0.71264366],
[1.14656623,  0.85612268, 1.35644848, 0.15593028],
[2.30138558,  0.03428558,  0.06794686,  0.94001399],
[0.09391892,  0.33946436, 0.4026701,   1.52602039]])

np_array = np_array.astype(np.float64)
ptr_np_array = np_array.ctypes.data_as(POINTER(c_double))

disp_matrix = cauction_lib.print_d_array
disp_matrix.argtypes = [ptr_array]
disp_matrix.restype = c_void_p

solve = cauction_lib.solve
solve.argtypes = [ptr_array, c_float]
solve.restype = ptr_assig_res

out = array(4, 4, ptr_np_array)
result = solve(out, 0.01)
output_array = np.ctypeslib.as_array(result.contents.agent_to_object, shape=(4,))

# out = load_matrix(c_int(2), c_int(4), ptr_np_array)
disp_matrix(out)
print(output_array)
# o = clean(out)





