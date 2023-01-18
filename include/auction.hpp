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

typedef assignment result[10000];

void bid(array unassigned_agents, array unassigned_objects, array bids);
void init_assignments(const size_t agents, const size_t objects, assignment **assig);
result *solve_jacobi(array *cost_matrix, const float eps);
#endif // ifndef _AUCTION_H