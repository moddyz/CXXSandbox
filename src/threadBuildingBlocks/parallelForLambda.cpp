#include <tbb/parallel_for.h>

#include <stdio.h>
#include <sstream>
#include <vector>

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
    tbb::parallel_for(tbb::blocked_range<int>(0, numbers.size()),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              numbers[i]++;
                          }
                      });
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
    std::vector<int> parallelArray(numElements, 1);
    ParallelForIncrement(parallelArray);

    return EXIT_SUCCESS;
}
