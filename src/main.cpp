#include <cstdio>
#include <cstdlib>
#include "array.hpp"
#include "auction.hpp"

int main()
{
	array *A;

	A = read_array(4, 4, "arr.txt");
	printf("Loaded array:\n");
	print_array(A);

	double *max1 = (double *)malloc(sizeof(double *));
	double *max2 = (double *)malloc(sizeof(double *));
	int *pos1 = (int *)malloc(sizeof(int *));
	int *pos2 = (int *)malloc(sizeof(int *));

	int line = 2;

	find_top2_with_pos_in_line(A, line, max1, max2, pos1, pos2);

	printf("Found top1 %f at %d and top2 %f at %d\n", *max1, *pos1, *max2, *pos2);

	return 0;
}