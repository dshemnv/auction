#ifndef _ARRAY_H
#define _ARRAY_H
#define ROWS 10
#define COLS 10

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct array
{
	size_t rows;
	size_t cols;
	double data[ROWS * COLS];
} array;

void print_array(array *array);
array *read_array(const size_t rows, const size_t cols, const char *array_file);
array *get_line(const char *instr, const size_t index, array *input_array);
void set_val(const size_t row, const size_t col, const int val, array *array);
double get_val(const size_t row, const size_t col, array *array);
void set_size(const size_t rows, const size_t cols, array *array);
void fill(array *array, const int value);
double max(array *array);

#endif
