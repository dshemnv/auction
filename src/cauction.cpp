#include "carray.hpp"
#include <new>

extern "C" {

void print_d_array(d_array *array) {
    printf("[");
    for (int i = 0; i < array->rows; i++) {
        for (int j = 0; j < array->cols; j++) {
            printf("%lf ", array->data[i * array->cols + j]);
        }
        if (i != array->rows - 1) {
            printf("\n");
        }
    }
    printf("]\n");
}

assignment_result *solve(d_array *cost_matrix, float eps) {
    // Convert d_array to array<double>
    int rows = cost_matrix->rows;
    int cols = cost_matrix->cols;
    array<double> cpp_cost_matrix;
    init<double>(&cpp_cost_matrix, rows, cols, 0);

    for (int i = 0; i < cpp_cost_matrix.rows; i++) {
        for (int j = 0; j < cpp_cost_matrix.cols; j++) {
            cpp_cost_matrix.data[i * cols + j] =
                cost_matrix->data[i * cols + j];
        }
    }
    // Init results
    assignments<double> result;
    assignment<double> assig = {.agent = -1, .object = -1, .value = -1};
    result.size = rows;
    result.is_empty = true;
    result.result = new (std::nothrow) assignment<double>[result.size];
    for (int i = 0; i < result.size; i++) {
        result.result[i] = assig;
    }
    // Solve using solve_jacobi
    solve_jacobi(&cpp_cost_matrix, eps, &result);
    // Convert assignments<double> to assignment_result
    int *agent_to_obj = (int *)malloc(result.size * sizeof(int));
    int *obj_to_agent = (int *)malloc(result.size * sizeof(int));
    for (int i = 0; i < result.size; i++) {
        int agent = result.result[i].agent;
        int object = result.result[i].object;
        if (agent != -1 && object != -1) {
            agent_to_obj[agent] = object;
            obj_to_agent[object] = agent;
        }
    }

    assignment_result *res =
        (assignment_result *)malloc(sizeof(assignment_result));

    res->len = result.size;
    res->agent_to_object = agent_to_obj;
    res->object_to_agent = obj_to_agent;

    return res;
}
}