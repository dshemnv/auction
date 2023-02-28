#include "array.hpp"
#include "auction.hpp"
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>

#define TYPE double
using namespace std;
int main(int argc, char *argv[]) {
    static int show;
    static struct option long_options[] = {
        {"rows", required_argument, NULL, 'r'},
        {"cols", required_argument, NULL, 'c'},
        {"array", required_argument, NULL, 'i'},
        {"epsilon", required_argument, NULL, 'e'},
        {"show", no_argument, &show, 1}};

    if (argc < 3) {
        cerr << "Usage: \n"
             << "\t" << argv[0] << " -r rows -c cols -i input_file -e epsilon"
             << endl;
        exit(EXIT_FAILURE);
    }

    int n_agents = 0;
    int n_objects = 0;
    double epsilon = 0;
    const char *path = NULL;

    int c;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "r:c:i:e:", long_options,
                            &option_index)) != -1) {
        switch (c) {
        case ('r'):
            n_agents = atoi(optarg);
            break;
        case ('c'):
            n_objects = atoi(optarg);
            break;
        case ('i'):
            path = optarg;
            break;
        case ('e'):
            epsilon = atof(optarg);
        default:
            break;
        }
    }

    array<TYPE> A;
    init<TYPE>(&A, n_agents, n_objects, 0);

    bool square = n_agents != n_objects ? false : true;

    read_array<TYPE>(path, &A);
    if (show == 1) {
        cout << "Loaded array:" << endl;
        print_array(&A);
    }
    // while (1) {

    assignments<TYPE> result;
    assignment<TYPE> assig = {.agent = -1, .object = -1, .value = -1};
    result.is_empty = true;
    result.size = n_agents;
    result.n_assignment = 0;
    result.result = new assignment<TYPE>[result.size];
    for (int i = 0; i < result.size; i++) {
        result.result[i] = assig;
    }

    clock_t t;
    t = clock();
    solve_jacobi<TYPE>(&A, epsilon, &result);
    t = clock() - t;

    double timing = (double)t / CLOCKS_PER_SEC;
    cout << setprecision(4);
    cout << "Assignment took " << timing * 1000.0 << "ms" << endl;

    array<int> agent_to_object;
    array<int> obj_to_agent;

    init<int>(&agent_to_object, 1, result.n_assignment, -1);
    init<int>(&obj_to_agent, 1, result.n_assignment, -1);

    assignements_to_arrays<TYPE>(&result, &agent_to_object, &obj_to_agent,
                                 square);
    if (show == 1) {
        print_array<int>(&agent_to_object);
        print_array<int>(&obj_to_agent);
    }
    delete[] agent_to_object.data;
    delete[] obj_to_agent.data;
    delete[] result.result;
    // }
    delete[] A.data;
    return EXIT_SUCCESS;
}
