#include <tbb/flow_graph.h>

#include "utils.h"

int main(int argc, char** argv)
{
    PROFILE_FUNCTION();

    // Construct a graph.
    tbb::flow::graph graph;

    // Construct a function node which adds 5 to a number.
    tbb::flow::function_node<int, int> addNode(
        graph, /* maxConcurrency*/ tbb::flow::unlimited, [](int input) -> int {
            printf("[Add] Consumed %i input\n", input);
            int output = input + 5;
            printf("[Add] Produced %i output\n", output);
            return output;
        });

    // Construct a function node which squares the number.
    tbb::flow::function_node<int, int> squareNode(
        graph, /* maxConcurrency*/ 1, [](int input) -> int {
            printf("[Square] Consumed %i input\n", input);
            int output = input * input;
            printf("[Square] Produced %i output\n", output);
            return output;
        });

    tbb::flow::make_edge(addNode, squareNode);

    // Schedule tasks on a few inputs.
    addNode.try_put(1);
    addNode.try_put(2);
    addNode.try_put(3);

    // Block until all the tasks finish executing.
    graph.wait_for_all();

    return EXIT_SUCCESS;
}
