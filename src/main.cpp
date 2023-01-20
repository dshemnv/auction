#include <cstdio>
#include <cstdlib>
#include "array.hpp"
#include "auction.hpp"

int main()
{
	array A;
	int n_agents = 4;
	int n_objects = 4;

	read_array(n_agents, n_objects, "/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/arr.txt", &A);
	printf("Loaded array:\n");
	print_array(A);

	assignments result;
	assignment assig = {.agent = -1, .object = -1, .value = -1};
	result.is_empty = true;
	result.size = n_agents;
	for (int i = 0; i < result.size; i++)
	{
		result.result[i] = assig;
	}
	solve_jacobi(&A, 0.03, &result);
	array agent_to_object, object_to_agent;
	assignements_to_arrays(&result, &agent_to_object, &object_to_agent);

	print_array(agent_to_object);
	return EXIT_SUCCESS;
}
