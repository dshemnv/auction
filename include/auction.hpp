#ifndef _AUCTION_H
#define _AUCTION_H

#include <cstdio>
#include <cstdlib>
#include <ctime>
#define NO_REPLACE -2
#define NOTHING_FOUND -1
#define MAX_ITER 1000
#include "array.hpp"

template <typename T = double> struct assignment {
    int agent;
    int object;
    T value;
};

template <typename T = double> struct assignments {
    bool is_empty;
    int size;
    int n_assignment;
    assignment<T> *result;
};

struct auction_process {
    array<bool> *rows;
    array<bool> *cols;
};

void reset_assignement(array<bool> *array_mask);
bool assignement_found(array<bool> *agents_mask);

template <typename T = double>
void validate_bid_and_value(array<int> *input_array, array<T> *bids_array,
                            array<bool> *array_mask, int value, int new_index,
                            T bid) {

    bool found = false;
    for (int i = 0; i < array_mask->cols; i++) {
        if (array_mask->data[i] == false) {
            continue;
        } else if (input_array->data[i] == value) {
            if (bid > bids_array->data[i]) {
                // printf(
                // "[C]: Bid already made on agent/object %d, resetting %d\n",
                // value, i);
                bids_array->data[i] = -1;
                array_mask->data[i] = false;
                input_array->data[i] = -1;
                found = true;
            } else {
                return;
            }
        }
        if (found) {
            break;
        }
    }
    // printf("[C]: Assigning agent/object %d to object/agent %d\n", new_index,
    //    value);
    bids_array->data[new_index] = bid;
    array_mask->data[new_index] = true;
    input_array->data[new_index] = value;
    return;
}

template <typename T = double>
void create_first_results(assignments<T> *result, array<int> *objects_array,
                          array<T> *bids_array, array<bool> *array_mask,
                          array<bool> *objects_mask, array<bool> *agents_mask) {

    for (int i = 0; i < array_mask->cols; i++) {
        assert((array_mask->data[i] == 0) || (array_mask->data[i] == 1));
        if (array_mask->data[i]) {
            int object_number = objects_array->data[i];
            T bid = bids_array->data[i];
            result->result[i].agent = i;
            result->result[i].object = object_number;
            result->result[i].value = bid;
            agents_mask->data[i] = true;
            objects_mask->data[object_number] = true;
            result->n_assignment += 1;
        }
    }
    result->is_empty = false;
}

template <typename T = double>
void update_object_assignment(assignments<T> *result, array<int> *object_array,
                              array<T> *bids_array, array<bool> *array_mask,
                              array<bool> *objects_mask,
                              array<bool> *agents_mask) {
    int available_idx;
    for (int j = 0; j < array_mask->cols; j++) { // Mask of assigned objects
        bool updated = false;
        T bid = bids_array->data[j];
        int new_object = object_array->data[j];
        if (!array_mask->data[j]) {
            continue;
        }
        for (int i = 0; i < result->size; i++) {
            if (object_array->data[j] == result->result[i].object) {
                // printf("[C]: Object %d is already present in assignement\n",
                //    object_array->data[j]);
                int old_agent = result->result[i].agent;
                int old_object = result->result[i].object;
                agents_mask->data[old_agent] = false;
                objects_mask->data[old_object] = false;

                result->result[i].agent = j;
                result->result[i].object = new_object;
                result->result[i].value = bid;
                // printf("[C]: Unassigned object %d from agent %d\n",
                // old_object,
                //    old_agent);
                // printf("[C]: Assigned object %d to agent %d\n", new_object,
                // j);
                updated = true;
            }
        }
        if (!updated) {
            // puts("[C]: Same assignement not found, searching for index to add
            // "
            //      "new assignment");
            find_available_idx(result, &available_idx);
            // printf("[C]: Found index %d\n", available_idx);
            assert(available_idx != -1);
            result->result[available_idx].agent = j;
            result->result[available_idx].object = object_array->data[j];
            result->result[available_idx].value = bids_array->data[j];
            result->n_assignment += 1;
        }
        agents_mask->data[j] = true;
        objects_mask->data[new_object] = true;
    }
}

template <typename T = double>
void find_available_idx(assignments<T> *result, int *idx) {
    *idx = -1;
    for (int i = 0; i < result->size; i++) {
        if (result->result[i].agent == -1) {
            *idx = i;
            return;
        }
    }
    return;
}

template <typename T = double>
void update_agent_assignment(assignments<T> *result, array<int> *agent_array,
                             array<T> *bids_array, array<bool> *array_mask,
                             array<bool> *objects_mask,
                             array<bool> *agents_mask) {
    int available_idx;
    for (int j = 0; j < array_mask->cols; j++) {
        bool updated = false;
        T bid = bids_array->data[j];
        int new_agent = agent_array->data[j];
        if (!array_mask->data[j]) {
            continue;
        }
        for (int i = 0; i < result->size; i++) {
            if (agent_array->data[j] == result->result[i].agent) {

                // printf("[C]: Agent %d is already present in assignement\n",
                //    agent_array->data[j]);
                int old_agent = result->result[i].agent;
                int old_object = result->result[i].object;
                agents_mask->data[old_agent] = false;
                objects_mask->data[old_object] = false;

                result->result[i].agent = new_agent;
                result->result[i].object = j;
                result->result[i].value = bid;
                // printf("[C]: Unassigned object %d from agent %d\n",
                // old_object,
                //    old_agent);
                // printf("[C]: Assigned object %d to agent %d\n", j,
                // new_agent);
                updated = true;
            }
        }
        if (!updated) {
            // puts("[C]: Same assignement not found, searching for index to add
            // "
            //      "new assignment");
            find_available_idx(result, &available_idx);
            // printf("[C]: Found index %d\n", available_idx);

            assert(available_idx != -1);
            result->result[available_idx].agent = new_agent;
            result->result[available_idx].object = j;
            result->result[available_idx].value = bid;
            result->n_assignment += 1;
        }
        agents_mask->data[new_agent] = true;
        objects_mask->data[j] = true;
    }
}

template <typename T = double>
void update_prices(assignments<T> *result, array<bool> *array_mask,
                   array<T> *prices) {
    int agent;
    int object;
    for (int i = 0; i < result->size; i++) {
        agent = result->result[i].agent;
        object = result->result[i].object;
        if (agent != -1 && array_mask->data[agent]) {
            prices->data[object] =
                prices->data[object] + result->result[i].value;
        }
    }
}

template <typename T = double>
void update_profits(assignments<T> *result, array<bool> *array_mask,
                    array<T> *profits) {
    int agent;
    int object;
    for (int i = 0; i < result->size; i++) {
        agent = result->result[i].agent;
        object = result->result[i].object;
        if (object != -1 && array_mask->data[object]) {
            profits->data[agent] =
                profits->data[agent] + result->result[i].value;
        }
    }
}

template <typename T = double>
void update_profits_after_forward(assignments<T> *result, array<T> *cost_matrix,
                                  array<T> *profits, array<T> *prices,
                                  array<bool> *array_mask) {
    int agent;
    int object;
    for (int i = 0; i < result->size; i++) {
        agent = result->result[i].agent;
        object = result->result[i].object;
        if (agent != -1 && array_mask->data[agent]) {
            profits->data[agent] =
                cost_matrix->data[agent * cost_matrix->cols + object] -
                prices->data[object];
        }
    }
}
template <typename T = double>
void update_prices_after_backward(assignments<T> *result, array<T> *cost_matrix,
                                  array<T> *profits, array<T> *prices,
                                  array<bool> *array_mask) {
    int agent;
    int object;
    for (int i = 0; i < result->size; i++) {
        agent = result->result[i].agent;
        object = result->result[i].object;
        if (object != -1 && array_mask->data[object]) {
            prices->data[object] =
                cost_matrix->data[agent * cost_matrix->cols + object] -
                profits->data[agent];
        }
    }
}

template <typename T = double>
bool check_eCS(array<T> *profits, array<T> *prices, array<T> *cost_matrix,
               const double eps, assignments<T> *S) {

    for (int i = 0; i < cost_matrix->rows; i++) {
        for (int j = 0; j < cost_matrix->cols; j++) {
            if (!(profits->data[i] + prices->data[j] >=
                  cost_matrix->data[i * cost_matrix->cols + j] - eps)) {
                return false;
            }
        }
    }

    for (int i = 0; i < S->size; i++) {
        int agent = S->result[i].agent;
        int object = S->result[i].object;
        if (agent != -1) {
            if (!(profits->data[agent] + prices->data[object] ==
                  cost_matrix->data[agent * cost_matrix->cols + object])) {
                return false;
            }
        }
    }

    return true;
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
        for (int j = 0; j < values->cols; j++) {
            values->data[j] =
                cost_matrix->data[i * cost_matrix->cols + j] - prices->data[j];
        }
        // puts("[C]: Row values");
        // print_array(values);
        T max1, max2;
        int pos1;
        find_top2_with_pos_in_row<T>(values, 0, &max1, &max2, &pos1);
        // assert(max1 != 0 && max2 != 0);
        double current_bid = max1 - max2 + eps;
        // printf("[C]: Found max1 : %lf and max2 : %lf, bid is %lf\n", max1,
        // max2,
        //        current_bid);
        validate_bid_and_value<T>(objects_array, bids_array, array_mask, pos1,
                                  i, current_bid);
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
        for (int i = 0; i < values->rows; i++) {
            values->data[i] =
                cost_matrix->data[i * cost_matrix->cols + j] - profits->data[i];
        }
        // puts("[C]: Col values");
        // print_array(values);
        // std::cout << "Diff for column " << j << std::endl;
        // print_array(values);
        T max1, max2;
        int pos1;
        find_top2_with_pos_in_col<T>(values, 0, &max1, &max2, &pos1);
        double current_bid = max1 - max2 + eps;
        // printf("[C]: Found max1 : %lf at %d and max2 : %lf, bid is %lf\n",
        // max1,
        //        pos1, max2, current_bid);
        // assert(max1 != 0 && max2 != 0);
        validate_bid_and_value(agents_array, bids_array, array_mask, pos1, j,
                               current_bid);
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
    // puts("  [C]: Solving matrix");
    // printf("With eps=%lf\n", eps);
    // print_array(cost_matrix);
    array<T> prices;
    init<T>(&prices, 1, n_objects, 0);
    array<T> profits;
    init<T>(&profits, n_agents, 1, 0);

    array<bool> assigned_agents;
    init<bool>(&assigned_agents, 1, n_agents, false);
    array<bool> assigned_objects;
    init<bool>(&assigned_objects, 1, n_objects, false);

    array<int> agent_top_obj;
    init<int>(&agent_top_obj, 1, n_agents, -1);
    array<T> agent_top_bid;
    init<T>(&agent_top_bid, 1, n_agents, -1);

    array<int> obj_top_agent;
    init<int>(&obj_top_agent, 1, n_objects, -1);
    array<T> obj_top_bid;
    init<T>(&obj_top_bid, 1, n_objects, -1);

    array<T> values_forward;
    init<T>(&values_forward, 1, n_objects, 0);
    array<T> values_backward;
    init<T>(&values_backward, n_agents, 1, 0);

    array<bool> assigned_rows;
    init<bool>(&assigned_rows, 1, n_agents, false);
    array<bool> assigned_cols;
    init<bool>(&assigned_cols, 1, n_objects, false);

    array<int> agent_to_object;
    array<int> object_to_agent;
    init<int>(&agent_to_object, 1, result->size, -1);
    init<int>(&object_to_agent, 1, result->size, -1);

    int n_loops = 0;
    int n_forward = 0;
    int n_backward = 0;

    while (true) {
        if (n_loops > MAX_ITER) {
            dump_array("dumped_array_v2.txt", cost_matrix);
            break;
        }
        forward<T>(cost_matrix, &assigned_rows, &prices, &assigned_agents,
                   &assigned_objects, &values_forward, &agent_top_obj,
                   &agent_top_bid, result, eps);
        n_forward += 1;

        update_prices<T>(result, &assigned_rows, &prices);
        // std::cout << "Loop: " << n_loops << std::endl;
        // assignements_to_arrays(result, &agent_to_object, &object_to_agent);
        // std::cout << "Assignement after forward\n";
        // print_array(&agent_to_object);
        // std::cout << "Prices\n";
        // print_array(&prices);

        update_profits_after_forward<T>(result, cost_matrix, &profits, &prices,
                                        &assigned_rows);

        // std::cout << "Profits after forward\n";
        // print_array(&profits);

        // printf("LOOP %d\n", n_loops);
        bool checked = // false;
            check_eCS<T>(&profits, &prices, cost_matrix, eps, result);
        // if (!checked) {
        //     printf("eCS not verified.\n");
        //     exit(EXIT_FAILURE);
        // }
        n_loops += 1;

        if (assignement_found(&assigned_objects)) {
            // printf("Assignement took %d forward loops and %d backward
            // loops.\n",
            //    n_forward, n_backward);
            break;
        }

        // TODO: Switch only if at least one assignement was made
        // TODO: Check eps-CS conditions (7.24-7.25)
        if (checked) {

            backward<T>(cost_matrix, &assigned_cols, &profits, &assigned_agents,
                        &assigned_objects, &values_backward, &obj_top_agent,
                        &obj_top_bid, result, eps);

            n_backward += 1;

            update_profits<T>(result, &assigned_cols, &profits);

            // assignements_to_arrays(result, &agent_to_object,
            // &object_to_agent); std::cout << "Assignement after backward\n";
            // print_array(&agent_to_object);
            // std::cout << "Profits\n";
            // print_array(&profits);

            update_prices_after_backward<T>(result, cost_matrix, &profits,
                                            &prices, &assigned_cols);

            if (assignement_found(&assigned_objects)) {
                // printf("Assignement took %d forward loops and %d backward
                // loops.\n",
                //        n_forward, n_backward);
                break;
            }
        }

        // reset_array<T>(&agent_top_bid, -1);
        // reset_array<int>(&agent_top_obj, -1);
        // reset_array<T>(&obj_top_bid, -1);
        // reset_array<int>(&obj_top_agent, -1);
        // reset_array<T>(&values_backward, 0);
        // reset_array<T>(&values_forward, 0);
        // reset_array<int>(&agent_to_object, -1);
        // reset_array<int>(&object_to_agent, -1);
    }
    // std::cout << "Asignment found in " << n_loops << " loops" << std::endl;
    // delete[] profits.data;
    delete[] prices.data;
    delete[] assigned_agents.data;
    delete[] assigned_objects.data;
    delete[] agent_top_obj.data;
    delete[] agent_top_bid.data;
    delete[] obj_top_agent.data;
    delete[] obj_top_bid.data;
    delete[] values_forward.data;
    delete[] values_backward.data;
    delete[] assigned_rows.data;
    delete[] assigned_cols.data;
    delete[] agent_to_object.data;
    delete[] object_to_agent.data;
}

template <typename T = double>
void assignements_to_arrays(assignments<T> *results,
                            array<int> *agent_to_object,
                            array<int> *object_to_agent, bool square) {
    assert(!results->is_empty);
    int agent, object;
    int next_agent_idx = 0;
    int next_obj_idx = 0;

    for (int i = 0; i < results->size; i++) {
        agent = results->result[i].agent;
        object = results->result[i].object;
        if (agent != -1 && object != -1) {
            if (!square) {
                agent_to_object->data[next_agent_idx++] = object;
                object_to_agent->data[next_obj_idx++] = agent;
            } else {
                agent_to_object->data[agent] = object;
                object_to_agent->data[object] = agent;
            }
        }
    }
}

#endif // ifndef _AUCTION_H