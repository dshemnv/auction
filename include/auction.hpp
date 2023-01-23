#ifndef _AUCTION_H
#define _AUCTION_H

#include <cstdlib>
#include <cstdio>
#include "array.hpp"
#include <ctime>

template <typename T = double>
struct assignment
{
	int agent;
	int object;
	T value;
};

template <typename T = double>
struct assignments
{
	bool is_empty;
	int size;
	assignment<T> result[N_MAX];
};

void reset_assignement(array<bool> *array_mask);

template <typename T = double>
int idx_max_bid_for_object(array<int> *objects_array, array<T> *bids_array, array<bool> *array_mask, int object, T value)
{
	for (int i = 0; i < array_mask->cols; i++)
	{
		if (array_mask->data[i] == false)
		{
			continue;
		}
		else if (objects_array->data[i] == object)
		{
			if (bids_array->data[i] > value)
			{
				return -2;
			}
			return i;
		}
	}
	return -1;
}

template <typename T = double>
void create_first_results(assignments<T> *result, array<int> *objects_array, array<T> *bids_array, array<bool> *array_mask, array<bool> *objects_mask, array<bool> *agents_mask)
{
	for (int i = 0; i < array_mask->cols; i++)
	{
		if (array_mask->data[i])
		{
			result->result[i].agent = i;
			result->result[i].object = objects_array->data[i];
			result->result[i].value = bids_array->data[i];
			agents_mask->data[i] = true;
			objects_mask->data[objects_array->data[i]] = true;
		}
	}
	result->is_empty = false;
}

template <typename T = double>
void update_assignment(assignments<T> *result, array<int> *object_array, array<T> *bids_array, array<bool> *array_mask, array<bool> *objects_mask, array<bool> *agents_mask)
{
	for (int j = 0; j < array_mask->cols; j++)
	{
		if (!array_mask->data[j])
		{
			continue;
		}
		for (int i = 0; i < result->size; i++)
		{
			if (object_array->data[j] == result->result[i].object || j == result->result[i].agent)
			{
				int old_agent = result->result[i].agent;
				int old_object = result->result[i].object;
				if (old_agent != -1)
				{
					// printf("Replacing agent %d that was assigned to object %d with agent %d\n", old_agent, old_object, i);
					agents_mask->data[old_agent] = false;
					objects_mask->data[old_object] = false;
				}
			}
			result->result[j].agent = j;
			result->result[j].object = object_array->data[j];
			result->result[j].value = bids_array->data[j];
			agents_mask->data[j] = true;
			objects_mask->data[object_array->data[j]] = true;
		}
	}
}

template <typename T = double>
void update_prices(assignments<T> *result, array<bool> *array_mask, array<T> *prices)
{
	for (int i = 0; i < array_mask->cols; i++)
	{
		if (array_mask->data[i])
		{
			prices->data[result->result[i].object] = result->result[i].value;
			// printf("Agent %d gets object %d\n", i, result->result[i].object);
		}
	}
	reset_assignement(array_mask);
}

template <typename T = double>
void solve_jacobi(array<T> *cost_matrix, const double eps, assignments<T> *result)
{
	int n_agents = cost_matrix->rows;
	int n_objects = cost_matrix->cols;

	array<T> prices;
	init<T>(&prices, 1, n_objects);
	array<T> values;
	init<T>(&values, 1, n_agents);

	array<bool> assigned_agents;
	init<bool>(&assigned_agents, 1, n_agents);
	array<bool> assigned_objects;
	init<bool>(&assigned_objects, 1, n_objects);
	array<bool> assignement_exists;
	init<bool>(&assignement_exists, 1, n_agents);

	array<int> agent_top_obj;
	init<int>(&agent_top_obj, 1, n_agents);

	array<T> agent_top_bid;
	init<T>(&agent_top_bid, 1, n_agents);

	int n_loops = 0;

	while (true)
	{
		//	* Step 1: compute bids for each unassigned agent
		//  ** - Compute (line - prices)
		//  ** - Find max and pos of (line - prices)
		//	** - Compute and store bid: max1 - max2 + eps
		for (int i = 0; i < n_agents; i++)
		{
			if (assigned_agents.data[i] == true)
			{
				continue;
			}
			for (int j = 0; j < n_objects; j++)
			{
				values.data[j] = cost_matrix->data[i * cost_matrix->cols + j] - prices.data[j];
			}
			double max1, max2;
			int pos1;
			find_top2_with_pos_in_row(&values, 0, &max1, &max2, &pos1);
			// printf("  Agent %d : %f %f pos associated to max1 %d\n", i, max1, max2, pos1);
			int add_index = idx_max_bid_for_object<T>(&agent_top_obj, &agent_top_bid, &assignement_exists, pos1, max1 - max2 + eps);
			if (add_index > 0)
			{
				agent_top_obj.data[add_index] = pos1;
				agent_top_bid.data[add_index] = max1 - max2 + eps;
				assignement_exists.data[add_index] = true;
			}
			else
			{
				if (add_index == -2)
				{
					continue;
				}
				agent_top_obj.data[i] = pos1;
				agent_top_bid.data[i] = max1 - max2 + eps;
				assignement_exists.data[i] = true;
			}
		}
		// * Step 2: Find best bid for each objet and associated agent
		// ** - Find best bid and associated index (which is the object)
		// ** - Update state with new prices and assignment
		if (result->is_empty == true)
		{
			create_first_results<T>(result, &agent_top_obj, &agent_top_bid, &assignement_exists, &assigned_objects, &assigned_agents);
		}
		else
		{
			update_assignment<T>(result, &agent_top_obj, &agent_top_bid, &assignement_exists, &assigned_objects, &assigned_agents);
		}
		update_prices<T>(result, &assignement_exists, &prices);

		bool assignment_done = assigned_agents.data[0];
		for (int i = 1; i < n_objects; i++)
		{
			assignment_done = assignment_done && assigned_agents.data[i];
		}
		if (assignment_done == true)
		{
			break;
		}
		n_loops += 1;
	}
	printf("Asignment found in %d loops\n", n_loops);
}

template <typename T = double>
void assignements_to_arrays(assignments<T> *results, array<double> *agent_to_object, array<double> *object_to_agent)
{
	assert(!results->is_empty);
	int agent, object;
	init<double>(agent_to_object, 1, results->size);
	init<double>(object_to_agent, 1, results->size);

	for (int i = 0; i < results->size; i++)
	{
		agent = results->result[i].agent;
		object = results->result[i].object;
		agent_to_object->data[agent] = object;
		object_to_agent->data[object] = agent;
	}
}

#endif // ifndef _AUCTION_H