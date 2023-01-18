#ifndef _AUCTION_H
#define _AUCTION_H

#include <cstdlib>
#include <cstdio>
#include "array.hpp"

typedef struct assignment
{
	int agent;
	int object;
} assignment;

typedef struct auction_state
{
	int unassigned_agents[N_MAX];
	int unassigned_objects[N_MAX];
	int n_unassigned_agents;
	int n_unassigned_objects;
	double bids[N_MAX];
	double prices[N_MAX];
	assignment current_assignment[N_MAX];
	assignment latest_assignment;
} auction_state;

auction_state *init_auction_state(int n_agents, int n_objects);
void solve_jacobi(array *cost_matrix, const double eps, auction_state *state);
#endif // ifndef _AUCTION_H