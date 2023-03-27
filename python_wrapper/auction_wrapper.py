from ctypes import *
import numpy as np
from scipy.sparse import random
from scipy.optimize import linear_sum_assignment
import time


class array(Structure):
    _fields_ = [("cols", c_int), ("rows", c_int), ("data", POINTER(c_double))]


class assignement_result(Structure):
    _fields_ = [("agent_to_object", POINTER(c_int)),
                ("row_idx", POINTER(c_int)), ("len", c_int)]


def convert_np_to_double_ptr(numpy_array: np.ndarray):
    cols, rows = numpy_array.shape
    arr = numpy_array.astype(np.float64)
    ptr_arr = arr.ctypes.data_as(POINTER(c_double))
    return ptr_arr, (cols, rows)


def convert_double_ptr_to_d_array(ptr_array, rows, cols):
    return array(cols=cols, rows=rows, data=ptr_array)


def convert_assignment_result_to_np(assignment_result, shape):
    return (np.ctypeslib.as_array(assignment_result.contents.agent_to_object,
                                  shape=shape),
            np.ctypeslib.as_array(assignment_result.contents.row_idx,
                                  shape=shape))


def load_from_txt(file_path):
    arr = []
    with open(file_path, "r") as f:
        for line in f.readlines():
            line = line.strip().split(" ")
            line = list(map(lambda x: float(x), line))
            arr.append(line)
    return np.array(arr)


def save_as_text(arr, path):
    rows, _ = arr.shape
    with open(path, "w") as f:
        for row in range(rows):
            for val in arr[row, :]:
                f.write(f"{val:.3f} ")
            if row != rows - 1:
                f.write("\n")


class CAuctionWrapper:

    def __init__(self, lib_path) -> None:
        self.lib = CDLL(lib_path)

    def solve_auction(self, cost_matrix: np.ndarray, eps: float = 0.01):
        c_solver = self.lib.solve
        c_solver.argtypes = [POINTER(array), c_float]
        c_solver.restype = POINTER(assignement_result)

        # print(" [Py]: Convert numpy to ptr to array of doubles")
        p_cost_matrix_d, (cols, rows) = convert_np_to_double_ptr(cost_matrix)
        # print(" [Py]: Convert ptr of doubles to ptr to structure d_array")
        c_cost_matrix = convert_double_ptr_to_d_array(p_cost_matrix_d, rows,
                                                      cols)

        # print(" [Py]: Calling C solver")
        t1 = time.perf_counter()
        result = c_solver(c_cost_matrix, eps)
        res_len = result.contents.len
        t = time.perf_counter() - t1
        # print(" [Py]: C solver end")
        return convert_assignment_result_to_np(result, (res_len, )), t


if __name__ == "__main__":
    c_auction = CAuctionWrapper(
        "/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/bin/cauction_lib.so"
    )
    # test_mat = np.array([[0.08411499, 0.92580858, 1.78757778,  0.71264366],
    # [1.14656623,  0.85612268, 1.35644848, 0.15593028],
    # [2.30138558,  0.03428558,  0.06794686,  0.94001399],
    # [0.09391892,  0.33946436, 0.4026701,   1.52602039]])

    # np.random.seed(10)
    # test_mat = np.abs(np.random.randn(20, 10))
    # test_mat = random(10, 5, density=0.3, dtype=np.double).A
    test_mat = np.array([
        0.1372, 0.1618, 0.3061, 0.6841, 0.5472, 0.3690, 0.1435, 0.0397, 0.4116,
        0.3493, 0.0091, 0.3735, 0.2744, 0.3746, 0.2428, 0.5538, 0.9817, 0.1421,
        0.3137, 0.2239, 0.0306, 0.2790, 0.9110, 0.5210, 0.7583, 0.7348, 0.7743,
        0.5141, 0.9566, 0.9800, 0.0712, 0.9028, 0.5192, 0.1781, 0.0758, 0.6138,
        0.8843, 0.6471, 0.6826, 0.5658, 0.4027, 0.2481, 0.9724, 0.0514, 0.5382,
        0.5018, 0.2959, 0.6144, 0.0743, 0.7504, 0.6775, 0.2388, 0.7409, 0.2098,
        0.5257, 0.8844, 0.7949, 0.9697, 0.1571, 0.9483, 0.7885, 0.5219, 0.1746,
        0.1443, 0.6790, 0.2993, 0.8411, 0.1382, 0.4657, 0.4445, 0.0944, 0.1640,
        0.8115, 0.8428, 0.7802, 0.4157, 0.3315, 0.0169, 0.2249, 0.0663, 0.8893,
        0.9257, 0.6597, 0.9940, 0.1634, 0.1567, 0.3830, 0.3556, 0.5084, 0.8769,
        0.8683, 0.0330, 0.3540, 0.9229, 0.8417, 0.9298, 0.2495, 0.2958, 0.2248,
        0.4147, 0.6101, 0.6265, 0.2594, 0.4640, 0.8959, 0.0499, 0.7655, 0.7090,
        0.2053, 0.1801, 0.8095, 0.3285, 0.8312, 0.8838, 0.9837, 0.6039, 0.0868,
        0.9623, 0.9720, 0.4381, 0.6158, 0.7346, 0.1474, 0.3987, 0.3255, 0.8916,
        0.7962, 0.4432, 0.1802, 0.7992, 0.5127, 0.8509, 0.2800, 0.7367, 0.4824,
        0.9903, 0.9967, 0.9325, 0.4963, 0.3982, 0.2393, 0.0219, 0.9742, 0.4583,
        0.4370, 0.5355, 0.5257, 0.5145, 0.2288, 0.6108, 0.5864, 0.1972, 0.7089,
        0.1086, 0.9868, 0.1111, 0.9803, 0.8097, 0.3205, 0.3614, 0.8006, 0.2119,
        0.7647, 0.6160, 0.5612, 0.0601, 0.5865, 0.8925, 0.5470, 0.0615, 0.6770,
        0.3463, 0.0801, 0.2084, 0.8953, 0.0475, 0.7990, 0.0562, 0.6983, 0.4510,
        0.8303, 0.9236, 0.9809, 0.0084, 0.9686, 0.9517, 0.0719, 0.5340, 0.7583,
        0.6409, 0.9709, 0.2224, 0.8244, 0.2963, 0.6795, 0.5574, 0.6962, 0.7569,
        0.3511, 0.0174
    ])
    # save_as_text(test_mat, "sparse_arr.txt")
    # test_mat = load_from_txt("sparse_arr.txt")
    # test_mat = load_from_txt("assym_array.txt")
    test_mat = np.reshape(test_mat, (10, 20))
    # print("[Py]: Loaded matrix from txt to np")

    print("[Py]: Calling solver")
    (agent_to_object,
     row_idx), c_auction_t = c_auction.solve_auction(test_mat, 0.0001)
    print("[Py]: Solver finished")
    t1 = time.perf_counter()
    rows, cols = linear_sum_assignment(-test_mat)
    hung_t = time.perf_counter() - t1

    print(test_mat)
    print("C Auction result:")
    print(f"Took {c_auction_t * 1000000:.3f} µs")
    print(row_idx)
    print(agent_to_object)
    print("Hungarian result")
    print(f"Took {hung_t * 1000000:.3f} µs")
    print(rows)
    print(cols)