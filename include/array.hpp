#ifndef _ARRAY_H
#define _ARRAY_H
#define N_MAX 100
#define ARR_MAX 10000.0

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

typedef struct array
{
	int rows;
	int cols;
	double data[N_MAX];
} array;

void print_array(array array);

void read_array(int rows, int cols, const char *array_file, array *output_array);

void find_top2_with_pos_in_row(array *array, int row, double *max1, double *max2, int *pos1, int *pos2);

void find_top1_with_pos_in_row(array *array, int row, double *max, int *pos);

void add_val_to_row(array *array, int row, double val);

void sub_val_to_row(array *array, int row, double val);

void sub_vals_to_arr_row(array *arr, int row, array *vals);

void find_top2_with_pos_in_col(array *array, int col, double *max1, double *max2, int *pos1, int *pos2);

void add_val_to_col(array *array, int col, double val);

void sub_val_to_col(array *array, int col, double val);

void set_size(const int rows, const int cols, array *array);

void fill(array *array, const double value);

#endif
