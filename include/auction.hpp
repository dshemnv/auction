#ifndef _AUCTION_H
#define _AUCTION_H

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "array.hpp"

template <typename T = double> struct assignment {
    int agent;
    int object;
    T value;
};

template <typename T = double> struct assignments {
    bool is_empty;
    int size;
    assignment<T> *result;
};

struct auction_process {
    array<bool> *rows;
    array<bool> *cols;
};

void reset_assignement(array<bool> *array_mask);
bool assignement_found(array<bool> *agents_mask);

template <typename T = double>
int get_max_idx_in_array_for_value(array<int> *input_array,
                                   array<T> *bids_array,
                                   array<bool> *array_mask, int compare_value,
                                   T bid_value) {
    for (int i = 0; i < array_mask->cols; i++) {
        if (array_mask->data[i] == false) {
            continue;
        } else if (input_array->data[i] == compare_value) {
            if (bids_array->data[i] > bid_value) {
                return -2;
            }
            return i;
        }
    }
    return -1;
}

template <typename T = double>
void create_first_results(assignments<T> *result, array<int> *objects_array,
                          array<T> *bids_array, array<bool> *array_mask,
                          array<bool> *objects_mask, array<bool> *agents_mask) {
    for (int i = 0; i < array_mask->cols; i++) {
        if (array_mask->data[i]) {
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
void update_object_assignment(assignments<T> *result, array<int> *object_array,
                              array<T> *bids_array, array<bool> *array_mask,
                              array<bool> *objects_mask,
                              array<bool> *agents_mask) {
    for (int j = 0; j < array_mask->cols; j++) {
        if (!array_mask->data[j]) {
            continue;
        }
        for (int i = 0; i < result->size; i++) {
            if (object_array->data[j] == result->result[i].object ||
                j == result->result[i].agent) {
                int old_agent = result->result[i].agent;
                int old_object = result->result[i].object;
                agents_mask->data[old_agent] = false;
                objects_mask->data[old_object] = false;

                result->result[i].agent = -1;
                result->result[i].object = -1;
                result->result[i].value = -1;
            }
        }
        result->result[j].agent = j;
        result->result[j].object = object_array->data[j];
        result->result[j].value = bids_array->data[j];
        agents_mask->data[j] = true;
        objects_mask->data[object_array->data[j]] = true;
    }
}

template <typename T = double>
void update_agent_assignment(assignments<T> *result, array<int> *agent_array,
                             array<T> *bids_array, array<bool> *array_mask,
                             array<bool> *objects_mask,
                             array<bool> *agents_mask) {
    for (int j = 0; j < array_mask->cols; j++) {
        if (!array_mask->data[j]) {
            continue;
        }
        for (int i = 0; i < result->size; i++) {
            if (agent_array->data[j] == result->result[i].agent ||
                j == result->result[i].object) {
                int old_agent = result->result[i].agent;
                int old_object = result->result[i].object;
                agents_mask->data[old_agent] = false;
                objects_mask->data[old_object] = false;

                result->result[i].agent = -1;
                result->result[i].object = -1;
                result->result[i].value = -1;
            }
        }
        result->result[j].agent = agent_array->data[j];
        result->result[j].object = j;
        result->result[j].value = bids_array->data[j];
        agents_mask->data[agent_array->data[j]] = true;
        objects_mask->data[j] = true;
    }
}

template <typename T = double>
void update_prices(assignments<T> *result, array<bool> *array_mask,
                   array<T> *prices) {
    for (int i = 0; i < prices->cols; i++) {
        if (array_mask->data[i]) {
            prices->data[result->result[i].object] += result->result[i].value;
            // printf("Agent %d gets object %d\n", i, result->result[i].object);
        }
    }
    reset_assignement(array_mask);
}

template <typename T = double>
void update_profits(assignments<T> *result, array<bool> *array_mask,
                    array<T> *profits) {
    for (int i = 0; i < profits->rows; i++) {
        if (array_mask->data[i]) {
            profits->data[result->result[i].object] += result->result[i].value;
        }
    }
    reset_assignement(array_mask);
}

template <typename T = double>
void forward(array<T> *cost_matrix, array<bool> *array_mask, array<T> *prices,
             array<bool> *agents_mask, array<bool> *objects_mask,
             array<T> *values, array<int> *objects_array, array<T> *bids_array,
             assignments<T> *result, const double eps) {
    //	* Step 1: compute bids for each unassigned agent
    //  ** - Compute (line - prices)
    //  ** - Find max and pos of (line - prices)
    //	** - Compute and store bid: max1 - max2 + eps

    for (int i = 0; i < cost_matrix->rows; i++) {
        if (agents_mask->data[i] == true) {
            continue;
        }
        for (int j = 0; j < cost_matrix->cols; j++) {
            values->data[j] =
                cost_matrix->data[i * cost_matrix->cols + j] - prices->data[j];
        }
        T max1, max2;
        int pos1;
        find_top2_with_pos_in_row<T>(values, 0, &max1, &max2, &pos1);
        double current_bid = max1 - max2 + eps;
        int add_index = get_max_idx_in_array_for_value<T>(
            objects_array, bids_array, array_mask, pos1, current_bid);
        if (add_index > 0) {
            objects_array->data[add_index] = pos1;
            bids_array->data[add_index] = max1 - max2 + eps;
            array_mask->data[add_index] = true;
        } else {
            if (add_index == -2) {
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
    if (result->is_empty == true) {
        create_first_results<T>(result, objects_array, bids_array, array_mask,
                                objects_mask, agents_mask);
    } else {
        update_object_assignment<T>(result, objects_array, bids_array,
                                    array_mask, objects_mask, agents_mask);
    }
}

template <typename T = double>
void backward(array<T> *cost_matrix, array<bool> *array_mask, array<T> *profits,
              array<bool> *agents_mask, array<bool> *objects_mask,
              array<T> *values, array<int> *agents_array, array<T> *bids_array,
              assignments<T> *result, const double eps) {
    //	* Step 1: compute bids for each unassigned object
    //  ** - Compute (column - profits)
    //  ** - Find max and pos of (column - profits)
    //	** - Compute and store bid: max1 - max2 + eps

    for (int j = 0; j < cost_matrix->cols; j++) {
        if (objects_mask->data[j] == true) {
            continue;
        }
        for (int i = 0; i < cost_matrix->rows; i++) {
            values->data[j] =
                cost_matrix->data[i * cost_matrix->cols + j] - profits->data[j];
        }
        // std::cout << "Diff for column " << j << std::endl;
        // print_array(values);
        T max1, max2;
        int pos1;
        find_top2_with_pos_in_col<T>(values, 0, &max1, &max2, &pos1);
        double current_bid = max1 - max2 + eps;
        int add_index = get_max_idx_in_array_for_value<T>(
            agents_array, bids_array, array_mask, pos1, current_bid);
        if (add_index > 0) {
            agents_array->data[add_index] = pos1;
            bids_array->data[add_index] = max1 - max2 + eps;
            array_mask->data[add_index] = true;
        } else {
            if (add_index == -2) {
                continue;
            }
            agents_array->data[j] = pos1;
            bids_array->data[j] = max1 - max2 + eps;
            array_mask->data[j] = true;
        }
    }
    // * Step 2: Find best bid for each agent and associated object
    // ** - Find best bid and associated index (which is the agent)
    // ** - Update state with new prices and assignment

    update_agent_assignment<T>(result, agents_array, bids_array, array_mask,
                               objects_mask, agents_mask);
}

template <typename T = double>
void solve_jacobi(array<T> *cost_matrix, const double eps,
                  assignments<T> *result) {
    int n_agents = cost_matrix->rows;
    int n_objects = cost_matrix->cols;

    array<T> prices;
    init<T>(&prices, 1, n_objects);
    array<T> profits;
    init<T>(&profits, n_agents, 1);

    array<bool> assigned_agents;
    init<bool>(&assigned_agents, 1, n_agents);
    array<bool> assigned_objects;
    init<bool>(&assigned_objects, 1, n_objects);

    array<int> agent_top_obj;
    init<int>(&agent_top_obj, 1, n_agents);

    array<T> agent_top_bid;
    init<T>(&agent_top_bid, 1, n_agents);

    array<T> values_forward;
    init<T>(&values_forward, 1, n_agents);
    array<T> values_backward;
    init<T>(&values_backward, n_agents, 1);

    array<bool> assigned_rows;
    init<bool>(&assigned_rows, 1, n_agents);
    array<bool> assigned_cols;
    init<bool>(&assigned_cols, 1, n_objects);

    // array<int> agent_to_object;
    // array<int> object_to_agent;
    // init<int>(&agent_to_object, 1, result->size);
    // init<int>(&object_to_agent, 1, result->size);

    int n_loops = 0;

    while (true) {
        forward<T>(cost_matrix, &assigned_rows, &prices, &assigned_agents,
                   &assigned_objects, &values_forward, &agent_top_obj,
                   &agent_top_bid, result, eps);

        update_prices<T>(result, &assigned_rows, &prices);
        // std::cout << "Loop: " << n_loops << std::endl;
        // assignements_to_arrays(result, &agent_to_object, &object_to_agent);
        // std::cout << "Assignement after forward\n";
        // print_array(&agent_to_object);

        if (assignement_found(&assigned_agents)) {
            break;
        }

        backward<T>(cost_matrix, &assigned_cols, &profits, &assigned_agents,
                    &assigned_objects, &values_backward, &agent_top_obj,
                    &agent_top_bid, result, eps);

        update_profits<T>(result, &assigned_cols, &profits);

        // assignements_to_arrays(result, &agent_to_object, &object_to_agent);
        // std::cout << "Assignement after backward\n";
        // print_array(&agent_to_object);

        if (assignement_found(&assigned_agents)) {
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
    delete[] values_forward.data;
    delete[] values_backward.data;
    delete[] assigned_rows.data;
    delete[] assigned_cols.data;
    // delete[] agent_to_object.data;
    // delete[] object_to_agent.data;
}

template <typename T = double>
void assignements_to_arrays(assignments<T> *results,
                            array<int> *agent_to_object,
                            array<int> *object_to_agent) {
    assert(!results->is_empty);
    int agent, object;

    for (int i = 0; i < results->size; i++) {
        agent = results->result[i].agent;
        object = results->result[i].object;
        if (agent != -1 && object != -1) {
            agent_to_object->data[agent] = object;
            object_to_agent->data[object] = agent;
        }
    }
}

#endif // ifndef _AUCTION_H