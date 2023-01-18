#ifndef _ARRAY_H
#define _ARRAY_H
#define ROWS 10
#define COLS 10
#define ARR_MAX 1000000000.0

#include <cstdlib>
#include <cstdio>
#include <cstring>

typedef struct array
{
	int rows;
	int cols;
	double data[ROWS * COLS];
} array;

void print_array(array *array);
array *read_array(const int rows, const int cols, const char *array_file);
void find_top2_with_pos_in_line(array *array, int line, double *max1, double *max2, int *pos1, int *pos2);
void set_size(const int rows, const int cols, array *array);
void fill(array *array, const double value);

#endif
