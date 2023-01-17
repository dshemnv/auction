#include "array.h"

void print_array(array *input_array)
{
	printf("[");
	for (size_t i = 0; i < input_array->rows; i++)
	{
		for (size_t j = 0; j < input_array->cols; j++)
		{
			if (i == 0)
			{
				printf("%.2lf  ", get_val(i, j, input_array));
			}
			else if (i == input_array->rows - 1 && j == input_array->cols - 1)
			{
				printf(" %.2lf", get_val(i, j, input_array));
			}
			else
			{
				printf(" %.2lf ", get_val(i, j, input_array));
			}
		}
		if (i < input_array->rows - 1)
		{
			printf("\n");
		}
	}
	printf("]\n");
}

array *get_line(const char *instr, const size_t index, array *input_array)
{
	size_t iterable_max;
	array *arr = malloc(sizeof(array));
	if (strcmp(instr, "row") == 0)
	{
		arr->rows = 1;
		arr->cols = input_array->cols;
		iterable_max = input_array->cols;
	}
	else if (strcmp(instr, "col") == 0)
	{
		arr->rows = input_array->rows;
		arr->cols = 1;
		iterable_max = input_array->rows;
	}
	else
	{
		perror("Instruction is row or col.\n");
	}

	for (size_t i = 0; i < iterable_max; i++)
	{
		if (strcmp(instr, "row") == 0)
		{
			arr->data[i] = get_val(index, i, input_array);
		}
		else if (strcmp(instr, "col") == 0)
		{
			arr->data[i] = get_val(i, index, input_array);
		}
		else
		{
			perror("Instruction is row or col.\n");
		}
	}
	return arr;
}

array *read_array(const size_t rows, const size_t cols, const char *array_file)
{
	array *ptr_array = malloc(sizeof(array));
	FILE *file;
	double tmp_val;

	ptr_array->cols = cols;
	ptr_array->rows = rows;

	file = fopen(array_file, "r");
	if (file == NULL)
	{
		perror("Incorrect file");
		return 0;
	}

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			fscanf(file, "%lf", &tmp_val);
			ptr_array->data[i * cols + j] = tmp_val;
		}
	}
	fclose(file);
	return ptr_array;
}

void set_val(const size_t row, const size_t col, const int val, array *array)
{
	array->data[row * array->cols + col] = val;
}

double get_val(const size_t row, const size_t col, array *array)
{
	return array->data[row * array->cols + col];
}

void fill(array *arr, const int val)
{
	for (size_t i = 0; i < arr->rows; i++)
	{
		for (size_t j = 0; j < arr->cols; j++)
		{
			set_val(i, j, val, arr);
		}
	}
}

double max(array *array)
{
	double max = -1000000.0;
	for (size_t i = 0; i < array->rows; i++)
	{
		for (size_t j = 0; j < array->cols; j++)
		{
			double val = get_val(i, j, array);
			if (val > max)
			{
				max = val;
			}
		}
	}
	return max;
}

void set_size(const size_t rows, const size_t cols, array *array)
{
	array->cols = cols;
	array->rows = rows;
}