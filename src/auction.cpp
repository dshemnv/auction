#include "auction.hpp"
#include <cmath>

auction_state *init_auction_state(int n_agents, int n_objects)
{
	auction_state *state = (auction_state *)malloc(sizeof(auction_state));
	assignment default_assignment;
	default_assignment.agent = 0;
	default_assignment.object = 0;
	state->n_unassigned_agents = n_agents;
	state->n_unassigned_objects = n_objects;

	for (int i = 0; i < n_objects; i++)
	{
		state->current_assignment[i] = default_assignment;
		state->unassigned_objects[i] = i;
		state->bids[i] = 0.f;
	}
	for (int i = 0; i < n_agents; i++)
	{
		state->unassigned_agents[i] = i;
		state->prices[i] = 0.f;
	}
	state->latest_assignment = default_assignment;
	return state;
}

void solve_jacobi(array *cost_matrix, const double eps, auction_state *state)
{
	int *ptr_unassigned_agent = state->unassigned_agents;
	double *ptr_prices = state->prices;
	array *array_of_interest = (array *)malloc(sizeof(*cost_matrix));
	memcpy(array_of_interest, cost_matrix, sizeof(*cost_matrix));
	for (int r = 0; r < array_of_interest->rows; r++)
	{
		sub_val_to_row(array_of_interest, r, ptr_prices[r]);
	}

	//	* Step 1: compute bids for each unassigned agent
	//  ** - Find max and pos of (array - prices)
	//	** - Compute and store bid: max1 - max2 + eps

	double *max1 = (double *)malloc(sizeof(double *));
	double *max2 = (double *)malloc(sizeof(double *));
	int *pos1 = (int *)malloc(sizeof(int *));
	int *pos2 = (int *)malloc(sizeof(int *));
	double bids[state->n_unassigned_agents];
	for (int i = 0; i < state->n_unassigned_agents; i++)
	{
		printf("Agent %d bidding\n", ptr_unassigned_agent[i]);
		find_top2_with_pos_in_row(array_of_interest, ptr_unassigned_agent[i], max1, max2, pos1, pos2);
		printf("Found %f as max1 and %f as max2\n", *max1, *max2);
		bids[ptr_unassigned_agent[i]] = *max1 - *max2 + eps;
	}

	memcpy(state->prices, bids, sizeof(bids));

	// * Step 2: Find best bid for each objet and associated agent
	// ** - Find best bid and associated index (which is the object)
	// ** - Update state with new prices and assignment
	free(max1);
	free(max2);
	free(pos1);
	free(pos2);
}