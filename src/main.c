#include <stdio.h>
#include <stdlib.h>
#include "array.h"

#define MAX_SIZE 1000000000000

array_2d *read_array(size_t rows, size_t cols, const char *array_file)
{
	array_2d *array = malloc(sizeof(array_2d));
	FILE *file;
	double tmp_val;

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
			(*array)[i * cols + j] = tmp_val;
		}
	}
	fclose(file);
	return array;
}

int main()
{
	array_2d *A;
	array_1d *a1;
	array_1d *a2;

	A = read_array(4, 4, "arr.txt");

	print_array(A);
	a1 = get_row(2, A);
	a2 = get_col(0, A);

	print_row(a1);
	print_row(a2);

	return 0;
}