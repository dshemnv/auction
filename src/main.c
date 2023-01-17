#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int main()
{
	array *A;
	array *a1;
	array *a2;

	A = read_array(4, 4, "arr.txt");

	print_array(A);
	a1 = get_line("col", 2, A);
	a2 = get_line("row", 0, A);

	print_array(a2);
	print_array(a1);
	return 0;
}