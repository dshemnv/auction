#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int main()
{
	array *A;
	array *a1;
	array *a2;

	A = read_array(4, 4, "arr.txt");
	printf("Loaded array:\n");
	print_array(A);

	a1 = get_line("col", 2, A);
	a2 = get_line("row", 0, A);

	printf("\nThird column\n");
	print_array(a1);
	printf("\nFirst row\n");
	print_array(a2);

	double m = max(A);

	printf("\nMax value of array: %lf\n", m);

	return 0;
}