// An empty graph.

#include <tbb/flow_graph.h>

#include "utils.h"

int main(int argc, char** argv)
{
    PROFILE_FUNCTION();

    tbb::flow::graph graph;
    graph.wait_for_all();
    return EXIT_SUCCESS;
}
