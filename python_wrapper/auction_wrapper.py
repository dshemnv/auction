from ctypes import *
import numpy as np
from scipy.sparse import random
from scipy.optimize import linear_sum_assignment
import time

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

def convert_np_to_double_ptr(numpy_array: np.ndarray):
    rows, cols = numpy_array.shape
    arr = numpy_array.astype(np.float64)
    ptr_arr = arr.ctypes.data_as(POINTER(c_double))
    return ptr_arr, (rows, cols)

def convert_double_ptr_to_d_array(ptr_array, rows, cols):
    return array(rows, cols, ptr_array)

def convert_assignment_result_to_np(assignment_result, shape):
    return (np.ctypeslib.as_array(assignment_result.contents.agent_to_object, shape=shape), np.ctypeslib.as_array(assignment_result.contents.object_to_agent, shape=shape))

def load_from_txt(file_path):
    arr = []
    with open(file_path, "r") as f:
        for line in f.readlines():
            line = line.strip().split(" ")
            line = list(map(lambda x: float(x), line))
            arr.append(line)
    return np.array(arr)

class CAuctionWrapper:
    
    def __init__(self, lib_path) -> None:
        self.lib = CDLL(lib_path)
        
    
    def solve_auction(self, cost_matrix: np.ndarray, eps: float=0.01 ):
        c_solver = self.lib.solve
        c_solver.argtypes = [POINTER(array), c_float]
        c_solver.restype = POINTER(assignement_result)
        
        # print(" [Py]: Convert numpy to ptr to array of doubles")
        p_cost_matrix_d, (rows, cols) = convert_np_to_double_ptr(cost_matrix)
        # print(" [Py]: Convert ptr of doubles to ptr to structure d_array")
        c_cost_matrix = convert_double_ptr_to_d_array(p_cost_matrix_d, rows, cols)
        
        # print(" [Py]: Calling C solver")
        t1 = time.perf_counter()
        result = c_solver(c_cost_matrix, eps)
        res_len = result.contents.len
        t = time.perf_counter() - t1
        # print(" [Py]: C solver end") 
        return convert_assignment_result_to_np(result, (res_len,)), t
        
        
        
if __name__ == "__main__":
    c_auction = CAuctionWrapper("/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/bin/cauction_lib.so")

    # test_array = np.array([[0.08411499, 0.92580858, 1.78757778,  0.71264366],
    # [1.14656623,  0.85612268, 1.35644848, 0.15593028],
    # [2.30138558,  0.03428558,  0.06794686,  0.94001399],
    # [0.09391892,  0.33946436, 0.4026701,   1.52602039]])

    np.random.seed(10)
    test_mat = random(10, 10, density=0.3, dtype=np.double).A
    # test_mat = load_from_txt("assym_array.txt")
    print("[Py]: Loaded matrix from txt to np")

    print("[Py]: Calling solver")
    (agent_to_object, obj_to_agent), c_auction_t = c_auction.solve_auction(test_mat, 0.01)
    print("[Py]: Solver finished")
    t1 = time.perf_counter()
    rows, cols = linear_sum_assignment(-test_mat)
    hung_t = time.perf_counter() - t1
    
    print(test_mat)
    print("C Auction result:")
    print(f"Took {c_auction_t * 1000:.3f} ms")
    print(obj_to_agent)
    print(agent_to_object)
    print("Hungarian result")
    print(f"Took {hung_t * 1000:.3f} ms")
    print(rows)
    print(cols)



