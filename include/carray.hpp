// ##################################
// C Interface for Python shared lib
// ##################################

#include "auction.hpp"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
typedef struct d_array {
    int rows;
    int cols;
    double *data;
} d_array;

typedef struct assignment_result {
    int *agent_to_object;
    int *object_to_agent;
    int len;
} assignment_result;

void print_d_array(d_array *array);
assignment_result *solve(d_array *cost_matrix, float eps);
}
// ##################################