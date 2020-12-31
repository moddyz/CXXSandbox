#include <tbb/flow_graph.h>

#include "utils.h"

int
main(int argc, char** argv)
{
    PROFILE_FUNCTION();

    // Construct a graph.
    tbb::flow::graph graph;

    // Create a broadcast node (which does not reserve messages).
    tbb::flow::broadcast_node<int> broadcastNode(graph);

    // Create input buffer nodes (which _does_ reserve messages).
    tbb::flow::buffer_node<int> inputBufferA(graph);
    tbb::flow::buffer_node<int> inputBufferB(graph);

    // Create a join node.
    using JoinType =
        tbb::flow::join_node<std::tuple<int, int>, tbb::flow::reserving>;
    JoinType joinNode(graph);

    // Create an output buffer node.
    tbb::flow::buffer_node<JoinType::output_type> outputBuffer(graph);

    // Make connections.
    tbb::flow::make_edge(broadcastNode, tbb::flow::input_port<0>(joinNode));
    tbb::flow::make_edge(inputBufferA, tbb::flow::input_port<0>(joinNode));
    tbb::flow::make_edge(inputBufferB, tbb::flow::input_port<1>(joinNode));
    tbb::flow::make_edge(joinNode, outputBuffer);

    // Block until all the tasks finish executing.
    graph.wait_for_all();

    //
    // Push some inputs.
    //

    // Broadcast does not reserve, since the join has not yet received a input
    // for port #1, the value will be discarded.
    broadcastNode.try_put(2);

    // Buffer nodes do reserve, 3 will be reserved in inputBufferA.
    inputBufferA.try_put(3);

    // Buffer nodes do reserve, 4 will be reserved in inputBufferB.
    inputBufferB.try_put(4);
    inputBufferB.try_put(7);

    // Wait for tasks to spawn.
    graph.wait_for_all();

    // Pull output value.  Should be (3, 4)
    JoinType::output_type outputValue;
    while (outputBuffer.try_get(outputValue)) {
        printf("join_node output == (%d,%d)\n",
               std::get<0>(outputValue),
               std::get<1>(outputValue));
    }

    // Pull inputBufferA (should be empty, because the reserved 3 was joined with the 4).
    int value;
    if (inputBufferA.try_get(value)) {
        printf("inputBufferA had %d\n", value);
    } else {
        printf("inputBufferA was empty\n");
    }

    // Pull inputBufferB, should be 7 because it was reserved in the buffer node as it
    // had nothing to join with.
    if (inputBufferB.try_get(value)) {
        printf("inputBufferB had %d\n", value);
    } else {
        printf("inputBufferB was empty\n");
    }

    return EXIT_SUCCESS;
}
