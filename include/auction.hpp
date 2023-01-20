#ifndef _AUCTION_H
#define _AUCTION_H

#include <cstdlib>
#include <cstdio>
#include "array.hpp"

typedef struct assignment
{
	int agent;
	int object;
	double value;
} assignment;

typedef struct assignments
{
	bool is_empty;
	int size;
	assignment result[N_MAX];
} assignments;

int idx_max_bid_for_object(int *objects_array, double *bids_array, bool *array_mask, int array_len, int object, double value);

void create_first_results(assignments *result, int *objects_array, double *bids_array, bool *array_mask, bool *objects_mask, bool *agents_mask, int n_agents);

void assignements_to_arrays(assignments *results, array *agent_to_object, array *object_to_agent);

void update_prices(assignments *result, bool *array_mask, double *prices, int n_agents);

void reset_assignement(bool *array_mask, int len);

void solve_jacobi(array *cost_matrix, const double eps, assignments *result);
#endif // ifndef _AUCTION_H