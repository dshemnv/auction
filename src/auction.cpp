#include "auction.hpp"
#include <cmath>
#include <cassert>

int idx_max_bid_for_object(int *objects_array, double *bids_array, bool *array_mask, int array_len, int object, double value)
{
	for (int i = 0; i < array_len; i++)
	{
		if (array_mask[i] == false)
		{
			continue;
		}
		else if (objects_array[i] == object)
		{
			if (bids_array[i] > value)
			{
				return -2;
			}
			else
			{
				return i;
			}
		}
	}
	return -1;
}

void create_first_results(assignments *result, int *objects_array, double *bids_array, bool *array_mask, bool *objects_mask, bool *agents_mask, int n_agents)
{
	for (int i = 0; i < n_agents; i++)
	{
		if (array_mask[i])
		{
			result->result[i].agent = i;
			result->result[i].object = objects_array[i];
			result->result[i].value = bids_array[i];
			agents_mask[i] = true;
			objects_mask[objects_array[i]] = true;
		}
	}
	result->is_empty = false;
}

void update_assignment(assignments *result, int *object_array, double *bids_array, bool *array_mask, bool *objects_mask, bool *agents_mask, int n_agents)
{
	for (int i = 0; i < n_agents; i++)
	{
		if (array_mask[i])
		{
			int old_agent = result->result[i].agent;
			int old_object = result->result[i].object;
			if (old_agent != -1)
			{
				printf("Replacing agent %d that was assigned to object %d with agent %d\n", old_agent, old_object, i);
				agents_mask[old_agent] = false;
				objects_mask[old_object] = false;
			}
			result->result[i].agent = i;
			result->result[i].object = object_array[i];
			result->result[i].value = bids_array[i];
			agents_mask[i] = true;
			objects_mask[i] = true;
		}
	}
}
void assignements_to_arrays(assignments *results, array *agent_to_object, array *object_to_agent)
{
	assert(!results->is_empty);
	int agent, object;
	agent_to_object->cols = results->size;
	object_to_agent->cols = results->size;
	agent_to_object->rows = 1;
	object_to_agent->rows = 1;
	for (int i = 0; i < results->size; i++)
	{
		agent = results->result[i].agent;
		object = results->result[i].object;
		agent_to_object->data[agent] = object;
		object_to_agent->data[object] = agent;
	}
}

void update_prices(assignments *result, bool *array_mask, double *prices, int n_agents)
{
	for (int i = 0; i < n_agents; i++)
	{
		if (array_mask[i])
		{
			prices[result->result[i].object] = result->result[i].value;
			printf("Agent %d gets object %d\n", i, result->result[i].object);
		}
	}
	reset_assignement(array_mask, n_agents);
}

void reset_assignement(bool *array_mask, int len)
{
	for (int i = 0; i < len; i++)
	{
		array_mask[i] = false;
	}
}

void solve_jacobi(array *cost_matrix, const double eps, assignments *result)
{
	int n_agents = cost_matrix->rows;
	int n_objects = cost_matrix->cols;

	double prices[n_objects] = {0};
	array values = {.rows = 1, .cols = n_agents};
	for (int i = 0; i < n_agents; i++)
	{
		values.data[i] = 0;
	}

	bool assigned_agent[n_agents] = {false};
	bool assigned_object[n_objects] = {false};

	int agent_top_obj[n_agents] = {0};
	double agent_top_bid[n_agents] = {0};

	int n_loops = 0;

	bool assignement_exists[n_agents] = {false};
	while (true)
	{
		//	* Step 1: compute bids for each unassigned agent
		//  ** - Compute (line - prices)
		//  ** - Find max and pos of (line - prices)
		//	** - Compute and store bid: max1 - max2 + eps
		for (int i = 0; i < n_agents; i++)
		{
			if (assigned_agent[i] == true)
			{
				continue;
			}
			for (int j = 0; j < n_objects; j++)
			{
				values.data[j] = cost_matrix->data[i * cost_matrix->cols + j] - prices[j];
			}
			double max1, max2;
			int pos1, pos2;
			find_top2_with_pos_in_row(&values, 0, &max1, &max2, &pos1, &pos2);
			printf("  Agent %d : %f %f pos associated to max1 %d\n", i, max1, max2, pos1);
			int add_index = idx_max_bid_for_object(agent_top_obj, agent_top_bid, assignement_exists, n_agents, pos1, max1 - max2 + eps);
			if (add_index > 0)
			{
				agent_top_obj[add_index] = pos1;
				agent_top_bid[add_index] = max1 - max2 + eps;
				assignement_exists[add_index] = true;
			}
			else
			{
				if (add_index == -2)
				{
					continue;
				}
				agent_top_obj[i] = pos1;
				agent_top_bid[i] = max1 - max2 + eps;
				assignement_exists[i] = true;
			}
		}
		// * Step 2: Find best bid for each objet and associated agent
		// ** - Find best bid and associated index (which is the object)
		// ** - Update state with new prices and assignment
		if (result->is_empty == true)
		{
			create_first_results(result, agent_top_obj, agent_top_bid, assignement_exists, assigned_object, assigned_agent, n_agents);
		}
		else
		{
			update_assignment(result, agent_top_obj, agent_top_bid, assignement_exists, assigned_object, assigned_agent, n_agents);
		}
		update_prices(result, assignement_exists, prices, n_agents);

		bool assignment_done = assigned_object[0];
		for (int i = 1; i < n_objects; i++)
		{
			assignment_done = assignment_done && assigned_object[i];
		}
		if (assignment_done == true)
		{
			break;
		}
		n_loops += 1;
	}
	printf("Asignment found in %d loops\n", n_loops);
}