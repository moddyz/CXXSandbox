#include <tbb/flow_graph.h>

#include "utils.h"

// Type definition for an atomic counter.
using Counter = tbb::atomic<size_t>;

// A global counter.
static Counter g_count{ 0 };

struct IncrementCounters
{
    IncrementCounters(Counter& count)
      : m_count(count)
    {}

    tbb::flow::continue_msg operator()(tbb::flow::continue_msg /* unused */)
    {
        g_count++;
        m_count++;
        return tbb::flow::continue_msg();
    }

    Counter& m_count;
};

int main(int argc, char** argv)
{
    PROFILE_FUNCTION();

    // Construct a graph.
    tbb::flow::graph graph;

    // Define node counters.
    Counter counterA{ 0 }, counterB{ 0 }, counterC{ 0 };

    // Define functions.
    tbb::flow::function_node<tbb::flow::continue_msg> incrementA(
        graph, tbb::flow::serial, IncrementCounters(counterA));
    tbb::flow::function_node<tbb::flow::continue_msg> incrementB(
        graph, tbb::flow::serial, IncrementCounters(counterB));
    tbb::flow::function_node<tbb::flow::continue_msg> incrementC(
        graph, tbb::flow::serial, IncrementCounters(counterC));

    //
    // Single push policy.
    // _Each_ input into the buffer node will be pushed to a _single_ downstream
    // node.
    //

    // Define a single buffer node, which uses a "single-push" policy,
    // even though we connect the buffer to all 3 function nodes.
    tbb::flow::buffer_node<tbb::flow::continue_msg> buffer(graph);
    tbb::flow::make_edge(buffer, incrementA);
    tbb::flow::make_edge(buffer, incrementB);
    tbb::flow::make_edge(buffer, incrementC);

    // Add some inputs.
    buffer.try_put(tbb::flow::continue_msg());
    buffer.try_put(tbb::flow::continue_msg());
    buffer.try_put(tbb::flow::continue_msg());

    // Block until all the tasks finish executing.
    graph.wait_for_all();

    // Print results.
    printf("After single-push, g_count == %lu, counterA==%lu, "
           "counterB==%lu, counterC==%lu\n",
           (size_t)g_count,
           (size_t)counterA,
           (size_t)counterB,
           (size_t)counterC);

    //
    // Broadcast push policy.
    // _Each_ input into the broadcast node will be pushed to _all_ downstream
    // nodes.
    //

    // Remove single-push edges.
    tbb::flow::remove_edge(buffer, incrementA);
    tbb::flow::remove_edge(buffer, incrementB);
    tbb::flow::remove_edge(buffer, incrementC);

    // Reset counters.
    g_count = 0;
    counterA = 0;
    counterB = 0;
    counterC = 0;

    // Define a single buffer node, which uses a "single-push" policy,
    // even though we connect the buffer to all 3 function nodes.
    tbb::flow::broadcast_node<tbb::flow::continue_msg> broadcast(graph);
    tbb::flow::make_edge(broadcast, incrementA);
    tbb::flow::make_edge(broadcast, incrementB);
    tbb::flow::make_edge(broadcast, incrementC);

    // Add some inputs.
    broadcast.try_put(tbb::flow::continue_msg());
    broadcast.try_put(tbb::flow::continue_msg());
    broadcast.try_put(tbb::flow::continue_msg());

    // Block until all the tasks finish executing.
    graph.wait_for_all();

    // Print results.
    printf("After broadcast-push, g_count == %lu, counterA==%lu, "
           "counterB==%lu, counterC==%lu\n",
           (size_t)g_count,
           (size_t)counterA,
           (size_t)counterB,
           (size_t)counterC);

    return EXIT_SUCCESS;
}
