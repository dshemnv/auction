#include "auction.hpp"
#include <cmath>

result *solve_jacobi(array *cost_matrix, const float eps)
{
	const size_t n_agents = cost_matrix->rows;
	const size_t n_objects = cost_matrix->cols;

	// array bids;
	array prices;

	// array unassigned_agents;

	set_size(1, n_objects, &prices);

	double max_size = fmax(n_agents, n_objects);

	result *res = (result *)malloc(max_size * sizeof(assignment));
	for (size_t i = 0; i < max_size; i++)
	{
		res[i]->agent = -1;
		res[i]->object = -1;
	}

	// while (1)
	// {
	// 	if (assignment_found(p_assig))
	// 	{
	// 		break;
	// 	}
	// }
	return res;
}