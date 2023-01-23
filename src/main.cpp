#include <cstdio>
#include <cstdlib>
#include "array.hpp"
#include "auction.hpp"

#define TYPE double

int main()
{
	array<TYPE> A;

	int n_agents = 10;
	int n_objects = 10;

	read_array(n_agents, n_objects, "/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/array.txt", &A);
	printf("Loaded array:\n");
	print_array(&A);

	assignments<TYPE> result;
	assignment<TYPE> assig = {.agent = -1, .object = -1, .value = -1};
	result.is_empty = true;
	result.size = n_agents;
	for (int i = 0; i < result.size; i++)
	{
		result.result[i] = assig;
	}
	clock_t t;
	t = clock();
	solve_jacobi(&A, 0.3, &result);
	t = clock() - t;
	double timing = (double)t / CLOCKS_PER_SEC;
	printf("Assignment took %f ms\n", timing * 1000.0);

	array<TYPE> agent_to_object;
	array<TYPE> obj_to_agent;

	assignements_to_arrays<TYPE>(&result, &agent_to_object, &obj_to_agent);
	print_array(&agent_to_object);
	return EXIT_SUCCESS;
}
