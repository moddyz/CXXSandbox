#include <tbb/tbb.h>
#include <stdio.h>
#include <sstream>

#include "profiler.h"

static void
SerialForIncrement(std::vector<int>& numbers)
{
    PROFILE_FUNCTION();
    for (size_t i = 0; i < numbers.size(); ++i) {
        numbers[i]++;
    }
}

static void
ParallelForIncrement(std::vector<int>& numbers)
{
    PROFILE_FUNCTION();
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbbParallelFor <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    std::stringstream ss;
    ss << argv[1];
    size_t numElements = 0;
    ss >> numElements;

    // Run serial computation.
    std::vector<int> serialArray(numElements, 1);
    SerialForIncrement(serialArray);

    // Run parallel computation.
    std::vector<int> serialArray(numElements, 1);
    SerialForIncrement(serialArray);

    return EXIT_SUCCESS;
}
