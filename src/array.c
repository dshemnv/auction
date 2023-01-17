#include "array.h"

void print_array(array *input_array)
{
	printf("[");
	for (int i = 0; i < input_array->rows; i++)
	{
		for (int j = 0; j < input_array->cols; j++)
		{
			if (i == 0)
			{
				printf("%.2lf  ", input_array->data[i * input_array->cols + j]);
			}
			else if (i == input_array->rows - 1 && j == input_array->cols - 1)
			{
				printf(" %.2lf", input_array->data[i * input_array->cols + j]);
			}
			else
			{
				printf(" %.2lf ", input_array->data[i * input_array->cols + j]);
			}
		}
		if (i < input_array->rows - 1)
		{
			printf("\n");
		}
	}
	printf("]\n");
}

array *get_line(const char *instr, const u_int index, array *input_array)
{
	array *arr = malloc(sizeof(array));
	if (strcmp(instr, "row"))
	{
		arr->cols = input_array->cols;
		arr->rows = 1;
	}
	else if (strcmp(instr, "col"))
	{
		arr->rows = input_array->rows;
		arr->cols = 1;
	}

	for (int i = 0; i < input_array->rows; i++)
	{
		if (strcmp(instr, "row"))
		{
			arr->data[i] = input_array->data[index * input_array->rows + i];
		}
		else if (strcmp(instr, "col"))
		{
			arr->data[i] = input_array->data[i * input_array->rows + index];
		}
		else
		{
			perror("Instruction is row or col.\n");
		}
	}
	return arr;
}

array *read_array(size_t rows, size_t cols, const char *array_file)
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