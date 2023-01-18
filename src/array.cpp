#include "array.hpp"

void print_array(array *input_array)
{
	double *ptr_arr = input_array->data;
	printf("[");
	for (int i = 0; i < input_array->rows; i++)
	{
		for (int j = 0; j < input_array->cols; j++)
		{
			if (i == 0)
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

array *read_array(const int rows, const int cols, const char *array_file)
{
	array *ptr_array = (array *)malloc(sizeof(array));
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

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(file, "%lf", &tmp_val);
			ptr_array->data[i * cols + j] = tmp_val;
		}
	}
	fclose(file);
	return ptr_array;
}

void fill(array *arr, const double val)
{
	double *ptr_tab = arr->data;
	for (int i = 0; i < arr->rows; i++)
	{
		for (int j = 0; j < arr->cols; j++)
		{
			ptr_tab[i * arr->cols + j] = val;
		}
	}
}

void find_top2_with_pos_in_line(array *array, int line, double *max1, double *max2, int *pos1, int *pos2)
{
	double *ptr_line = array->data + line * array->cols;
	float max_val1 = ptr_line[0] > ptr_line[1] ? ptr_line[0] : ptr_line[1];
	float max_val2 = ptr_line[0] < ptr_line[1] ? ptr_line[0] : ptr_line[1];

	float pos_val1 = ptr_line[0] > ptr_line[1] ? 0 : 1;
	float pos_val2 = ptr_line[0] < ptr_line[1] ? 0 : 1;

	for (int i = 0; i < array->cols; i++)
	{
		if (ptr_line[i] > max_val1)
		{
			max_val2 = max_val1;
			pos_val2 = pos_val1;

			max_val1 = ptr_line[i];
			pos_val1 = i;
		}
		else if (ptr_line[i] > max_val2)
		{
			max_val2 = ptr_line[i];
			pos_val2 = i;
		}
	}
	*max1 = max_val1;
	*max2 = max_val2;
	*pos1 = pos_val1;
	*pos2 = pos_val2;
}

void set_size(const int rows, const int cols, array *array)
{
	array->cols = cols;
	array->rows = rows;
}