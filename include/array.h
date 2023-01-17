#ifndef _ARRAY_H
#define _ARRAY_H
#define ROWS 10
#define COLS 10

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct array
{
	u_int rows;
	u_int cols;
	double data[ROWS * COLS];
} array;

void print_array(array *array);
array *read_array(size_t rows, size_t cols, const char *array_file);
array *get_line(const char *instr, const u_int index, array *input_array);

#endif
