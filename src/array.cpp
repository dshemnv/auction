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
		exit(EXIT_FAILURE);
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

void find_top2_with_pos_in_row(array *array, int row, double *max1, double *max2, int *pos1, int *pos2)
{
	if (row > array->rows - 1)
	{
		fprintf(stderr, "Row %d is out of bounds. Arrays has %d rows\n ", row, array->rows);
		exit(EXIT_FAILURE);
	}
	double *ptr_row = array->data + row * array->cols;
	float max_val1 = ptr_row[0] > ptr_row[1] ? ptr_row[0] : ptr_row[1];
	float max_val2 = ptr_row[0] < ptr_row[1] ? ptr_row[0] : ptr_row[1];

	int pos_val1 = ptr_row[0] > ptr_row[1] ? 0 : 1;
	int pos_val2 = ptr_row[0] < ptr_row[1] ? 0 : 1;

	for (int i = 0; i < array->cols; i++)
	{
		if (ptr_row[i] > max_val1)
		{
			max_val2 = max_val1;
			pos_val2 = pos_val1;

			max_val1 = ptr_row[i];
			pos_val1 = i;
		}
		else if (ptr_row[i] > max_val2)
		{
			max_val2 = ptr_row[i];
			pos_val2 = i;
		}
	}
	*max1 = max_val1;
	*max2 = max_val2;
	*pos1 = pos_val1;
	*pos2 = pos_val2;
}

void find_top2_with_pos_in_col(array *array, int col, double *max1, double *max2, int *pos1, int *pos2)
{
	if (col > array->cols - 1)
	{
		fprintf(stderr, "Column %d is out of bounds. Arrays has %d columns\n ", col, array->cols);
		exit(EXIT_FAILURE);
	}
	double *ptr_col = array->data;
	int pos_val1 = ptr_col[col] > ptr_col[array->cols + col] ? col : array->cols + col;
	int pos_val2 = ptr_col[col] < ptr_col[array->cols + col] ? col : array->cols + col;

	double max_val1 = ptr_col[pos_val1] > ptr_col[pos_val2] ? ptr_col[pos_val1] : ptr_col[pos_val2];
	double max_val2 = ptr_col[pos_val1] < ptr_col[pos_val2] ? ptr_col[pos_val1] : ptr_col[pos_val2];

	for (int i = 0; i < array->rows; i++)
	{
		if (ptr_col[i * array->cols + col] > max_val1)
		{
			max_val2 = max_val1;
			pos_val2 = pos_val1;

			max_val1 = ptr_col[i * array->cols + col];
			pos_val1 = i;
		}
		else if (ptr_col[i * array->cols + col] > max_val2)
		{
			max_val2 = ptr_col[i * array->cols + col];
			pos_val2 = i;
		}
	}
	*max1 = max_val1;
	*max2 = max_val2;
	*pos1 = pos_val1;
	*pos2 = pos_val2;
}

void add_val_to_row(array *array, int row, double val)
{
	double *ptr_row = array->data + row * array->cols;
	for (int i = 0; i < array->cols; i++)
	{
		ptr_row[i] += val;
	}
}

void sub_val_to_row(array *array, int row, double val)
{
	double *ptr_row = array->data + row * array->cols;
	for (int i = 0; i < array->cols; i++)
	{
		ptr_row[i] -= val;
	}
}

void sub_vals_to_arr_row(array *arr, int row, array *vals, double *res)
{
	double *ptr_arr = arr->data;
	double *ptr_vals = vals->data;

	if (arr->cols != vals->cols)
	{
		fprintf(stderr, "Row should have the same lenght as array. Arrays has %d columns, row has %d columns.\n", arr->cols, vals->cols);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < arr->cols; i++)
	{
		res[i] = ptr_arr[row * arr->cols + i] - ptr_vals[i];
	}
}

void add_val_to_col(array *array, int col, double val)
{
	double *ptr_col = array->data;
	for (int i = 0; i < array->rows; i++)
	{
		ptr_col[i * array->cols + col] += val;
	}
}

void sub_val_to_col(array *array, int col, double val)
{
	double *ptr_col = array->data;
	for (int i = 0; i < array->rows; i++)
	{
		ptr_col[i * array->cols + col] -= val;
	}
}

void set_size(const int rows, const int cols, array *array)
{
	array->cols = cols;
	array->rows = rows;
}