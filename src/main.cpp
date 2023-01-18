#include <cstdio>
#include <cstdlib>
#include "array.hpp"
#include "auction.hpp"

int main()
{
	array *A;
	auction_state *state;
	int n_agents = 4;
	int n_objects = 4;

	state = init_auction_state(n_agents, n_objects);

	A = read_array(4, 4, "/home/dshem/Documents/these/ressources/papers/rapido2023/experiments/auction_c/arr.txt");
	printf("Loaded array:\n");
	print_array(A);

	double *max1 = (double *)malloc(sizeof(double *));
	double *max2 = (double *)malloc(sizeof(double *));
	int *pos1 = (int *)malloc(sizeof(int *));
	int *pos2 = (int *)malloc(sizeof(int *));

	solve_jacobi(A, 0.03, state);

	for (int i = 0; i < state->n_unassigned_agents; i++)
	{
		printf("Agent %d bidded %lf\n", i, state->prices[i]);
	}

	return 0;
}