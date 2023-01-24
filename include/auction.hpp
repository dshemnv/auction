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
	assignment<T> *result;
};

void reset_assignement(array<bool> *array_mask);
bool assignement_found(array<bool> *agents_mask);

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
void forward(array<T> *cost_matrix, array<bool> *array_mask, array<T> *prices, array<bool> *agents_mask, array<bool> *objects_mask, array<T> *values, array<int> *objects_array, array<T> *bids_array, assignments<T> *result, const double eps)
{
	//	* Step 1: compute bids for each unassigned agent
	//  ** - Compute (line - prices)
	//  ** - Find max and pos of (line - prices)
	//	** - Compute and store bid: max1 - max2 + eps

	for (int i = 0; i < cost_matrix->rows; i++)
	{
		if (agents_mask->data[i] == true)
		{
			continue;
		}
		for (int j = 0; j < cost_matrix->cols; j++)
		{
			values->data[j] = cost_matrix->data[i * cost_matrix->cols + j] - prices->data[j];
		}
		T max1, max2;
		int pos1;
		find_top2_with_pos_in_row<T>(values, 0, &max1, &max2, &pos1);
		double current_bid = max1 - max2 + eps;
		int add_index = idx_max_bid_for_object<T>(objects_array, bids_array, array_mask, pos1, current_bid);
		if (add_index > 0)
		{
			objects_array->data[add_index] = pos1;
			bids_array->data[add_index] = max1 - max2 + eps;
			array_mask->data[add_index] = true;
		}
		else
		{
			if (add_index == -2)
			{
				continue;
			}
			objects_array->data[i] = pos1;
			bids_array->data[i] = max1 - max2 + eps;
			array_mask->data[i] = true;
		}
	}
	// * Step 2: Find best bid for each objet and associated agent
	// ** - Find best bid and associated index (which is the object)
	// ** - Update state with new prices and assignment
	if (result->is_empty == true)
	{
		create_first_results<T>(result, objects_array, bids_array, array_mask, objects_mask, agents_mask);
	}
	else
	{
		update_assignment<T>(result, objects_array, bids_array, array_mask, objects_mask, agents_mask);
	}
}

template <typename T = double>
void solve_jacobi(array<T> *cost_matrix, const double eps, assignments<T> *result)
{
	int n_agents = cost_matrix->rows;
	int n_objects = cost_matrix->cols;

	array<T> prices;
	init<T>(&prices, 1, n_objects);

	array<bool> assigned_agents;
	init<bool>(&assigned_agents, 1, n_agents);
	array<bool> assigned_objects;
	init<bool>(&assigned_objects, 1, n_objects);

	array<int> agent_top_obj;
	init<int>(&agent_top_obj, 1, n_agents);

	array<T> agent_top_bid;
	init<T>(&agent_top_bid, 1, n_agents);

	array<T> values;
	init<T>(&values, 1, n_agents);

	array<bool> assignement_exists;
	init<bool>(&assignement_exists, 1, n_agents);
	int n_loops = 0;

	while (true)
	{
		forward<T>(cost_matrix, &assignement_exists, &prices, &assigned_agents, &assigned_objects, &values, &agent_top_obj, &agent_top_bid, result, eps);

		update_prices<T>(result, &assignement_exists, &prices);

		if (assignement_found(&assigned_agents))
		{
			break;
		}
		n_loops += 1;
	}
	std::cout << "Asignment found in " << n_loops << " loops" << std::endl;
	delete[] prices.data;
	delete[] assigned_agents.data;
	delete[] assigned_objects.data;
	delete[] agent_top_obj.data;
	delete[] agent_top_bid.data;
	delete[] values.data;
	delete[] assignement_exists.data;
}

template <typename T = double>
void assignements_to_arrays(assignments<T> *results, array<T> *agent_to_object, array<T> *object_to_agent)
{
	assert(!results->is_empty);
	int agent, object;
	init<T>(agent_to_object, 1, results->size);
	init<T>(object_to_agent, 1, results->size);

	for (int i = 0; i < results->size; i++)
	{
		agent = results->result[i].agent;
		object = results->result[i].object;
		agent_to_object->data[agent] = object;
		object_to_agent->data[object] = agent;
	}
}

#endif // ifndef _AUCTION_H