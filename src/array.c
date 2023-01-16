#include "array.h"
#include <stdio.h>
#include <stdlib.h>

void print_row(array_1d *array)
{
	for (int i = 0; i < COLS; i++)
	{
		printf("%.2f ", (*array)[i]);
	}
	printf("\n");
}

void print_array(array_2d *array)
{
	printf("[");
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i == 0)
			{
				printf("%.2lf  ", (*array)[i * COLS + j]);
			}
			else if (i == ROWS - 1 && j == COLS - 1)
			{
				printf(" %.2lf", (*array)[i * COLS + j]);
			}
			else
			{
				printf(" %.2lf ", (*array)[i * COLS + j]);
			}
		}
		if (i < ROWS - 1)
		{
			printf("\n");
		}
	}
	printf("]\n");
}

array_1d *get_row(int row_index, array_2d *array)
{
	array_1d *arr = malloc(sizeof(array_1d));
	for (int i = 0; i < COLS; i++)
	{
		(*arr)[i] = (*array)[row_index * COLS + i];
	}
	return arr;
}

array_1d *get_col(int col_index, array_2d *array)
{
	array_1d *arr = malloc(sizeof(array_1d));
	for (int i = 0; i < ROWS; i++)
	{
		(*arr)[i] = (*array)[i * COLS + col_index];
	}
	return arr;
}