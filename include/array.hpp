#ifndef _ARRAY_H
#define _ARRAY_H
#define N_MAX 200
#define ARR_MAX 10000.0

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cmath>
#include <cassert>

template <typename T = double>
struct array
{
	int rows;
	int cols;
	T data[N_MAX];
};

template <typename T>
void print_array(array<T> *input_array)
{
	T *ptr_arr = input_array->data;
	printf("[");
	for (int i = 0; i < input_array->rows; i++)
	{
		for (int j = 0; j < input_array->cols; j++)
		{
			if (i == 0 && j != input_array->cols - 1)
			{
				printf("%.2lf  ", ptr_arr[i * input_array->cols + j]);
			}
			else if (i == input_array->rows - 1 && j == input_array->cols - 1)
			{
				printf(" %.2lf", ptr_arr[i * input_array->cols + j]);
			}
			else
			{
				printf(" %.2lf ", ptr_arr[i * input_array->cols + j]);
			}
		}
		if (i < input_array->rows - 1)
		{
			printf("\n");
		}
	}
	printf("]\n");
}

template <typename T>
void set_size(const int rows, const int cols, array<T> *array)
{
	array->cols = cols;
	array->rows = rows;
}

template <typename T = double>
void read_array(int rows, int cols, const char *array_file, array<T> *output_array)
{
	FILE *file;
	T tmp_val;

	output_array->cols = cols;
	output_array->rows = rows;

	file = fopen(array_file, "r");
	if (file == NULL)
	{
		perror("Incorrect file");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(file, "%lf", &tmp_val);
			output_array->data[i * cols + j] = tmp_val;
		}
	}
	fclose(file);
}

template <typename T = double>
void fill(array<T> *arr, const T val)
{
	T *ptr_tab = arr->data;
	for (int i = 0; i < arr->rows; i++)
	{
		for (int j = 0; j < arr->cols; j++)
		{
			ptr_tab[i * arr->cols + j] = val;
		}
	}
}

template <typename T = double>
void init(array<T> *array, const int rows, const int cols)
{
	T fill_val = 0;
	set_size(rows, cols, array);
	fill<T>(array, fill_val);
}

template <typename T = double>
void find_top2_with_pos_in_row(array<T> *array, int row, T *max1, T *max2, int *pos_max)
{
	if (row >= array->rows)
	{
		fprintf(stderr, "Row %d is out of bounds. Arrays has %d rows\n ", row, array->rows);
		exit(EXIT_FAILURE);
	}
	T *ptr_row = array->data + row * array->cols;
	T max_val1 = ptr_row[0] > ptr_row[1] ? ptr_row[0] : ptr_row[1];
	T max_val2 = ptr_row[0] < ptr_row[1] ? ptr_row[0] : ptr_row[1];

	int pos_val = ptr_row[0] > ptr_row[1] ? 0 : 1;

	for (int i = 2; i < array->cols; i++)
	{
		if (ptr_row[i] > max_val1)
		{
			max_val2 = max_val1;

			max_val1 = ptr_row[i];
			pos_val = i;
		}
		else if (ptr_row[i] > max_val2)
		{
			max_val2 = ptr_row[i];
		}
	}
	assert(max_val1 >= max_val2);
	*max1 = max_val1;
	*max2 = max_val2;
	*pos_max = pos_val;
}

template <typename T = double>
void find_top1_with_pos_in_row(array<T> *array, int row, T *max, int *pos)
{
	T *ptr_row = array->data + row * array->cols;
	T max_val = ptr_row[0];
	int pos_val = 0;

	for (int i = 1; i < array->cols; i++)
	{
		if (ptr_row[i] > max_val)
		{
			max_val = ptr_row[i];
			pos_val = i;
		}
	}
	*max = max_val;
	*pos = pos_val;
}

template <typename T = double>
void find_top2_with_pos_in_col(array<T> *array, int col, T *max1, T *max2, int *pos1)
{
	if (col > array->cols - 1)
	{
		fprintf(stderr, "Column %d is out of bounds. Arrays has %d columns\n ", col, array->cols);
		exit(EXIT_FAILURE);
	}
	T *ptr_col = array->data;
	int pos_val1 = ptr_col[col] > ptr_col[array->cols + col] ? col : array->cols + col;
	int pos_val2 = ptr_col[col] < ptr_col[array->cols + col] ? col : array->cols + col;

	T max_val1 = ptr_col[pos_val1] > ptr_col[pos_val2] ? ptr_col[pos_val1] : ptr_col[pos_val2];
	T max_val2 = ptr_col[pos_val1] < ptr_col[pos_val2] ? ptr_col[pos_val1] : ptr_col[pos_val2];

	for (int i = 0; i < array->rows; i++)
	{
		if (ptr_col[i * array->cols + col] > max_val1)
		{
			max_val2 = max_val1;

			max_val1 = ptr_col[i * array->cols + col];
			pos_val1 = i;
		}
		else if (ptr_col[i * array->cols + col] > max_val2)
		{
			max_val2 = ptr_col[i * array->cols + col];
		}
	}
	*max1 = max_val1;
	*max2 = max_val2;
	*pos1 = pos_val1;
}

template <typename T = double>
void add_val_to_row(array<T> *array, int row, T val)
{
	T *ptr_row = array->data + row * array->cols;
	for (int i = 0; i < array->cols; i++)
	{
		ptr_row[i] += val;
	}
}

template <typename T = double>
void sub_val_to_row(array<T> *array, int row, T val)
{
	T *ptr_row = array->data + row * array->cols;
	for (int i = 0; i < array->cols; i++)
	{
		ptr_row[i] -= val;
	}
}

template <typename T = double>
void sub_vals_to_arr_row(array<T> *arr, int row, array<T> *vals)
{
	T *ptr_arr = arr->data;
	T *ptr_vals = vals->data;

	if (arr->cols != vals->cols)
	{
		fprintf(stderr, "Row should have the same lenght as array. Arrays has %d columns, row has %d columns.\n", arr->cols, vals->cols);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < arr->cols; i++)
	{
		ptr_arr[row * arr->cols + i] = ptr_arr[row * arr->cols + i] - ptr_vals[i];
	}
}

template <typename T = double>
void add_val_to_col(array<T> *array, int col, T val)
{
	T *ptr_col = array->data;
	for (int i = 0; i < array->rows; i++)
	{
		ptr_col[i * array->cols + col] += val;
	}
}

template <typename T = double>
void sub_val_to_col(array<T> *array, int col, T val)
{
	T *ptr_col = array->data;
	for (int i = 0; i < array->rows; i++)
	{
		ptr_col[i * array->cols + col] -= val;
	}
}

#endif
