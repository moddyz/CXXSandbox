#include <tbb/flow_graph.h>

#include "utils.h"

int
main(int argc, char** argv)
{
    PROFILE_FUNCTION();

    // Construct a graph.
    tbb::flow::graph graph;

    // Construct a "squaring" function_node with 1 int input, and 1 int output.
    tbb::flow::function_node<int, int> node(graph, 1, [](int input) -> int {
        printf("Consumed %i input\n", input);
        int output = input * input;
        printf("Produced %i output\n", output);
        return output;
    });

    // Schedule tasks on a few inputs.
    node.try_put(1);
    node.try_put(2);
    node.try_put(3);

    // Block until all the tasks finish executing.
    graph.wait_for_all();

    return EXIT_SUCCESS;
}

