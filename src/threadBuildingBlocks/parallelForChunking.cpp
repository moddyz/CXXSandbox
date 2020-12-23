#include <tbb/parallel_for.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

static void
SerialForIncrement(std::vector<int>& numbers)
{
    PROFILE_FUNCTION();
    for (size_t i = 0; i < numbers.size(); ++i) {
        numbers[i]++;
    }
}

static void
ParallelForIncrement(int grainSize, std::vector<int>& numbers)
{
    PROFILE_FUNCTION();
    tbb::parallel_for(
        tbb::blocked_range<int>(0, numbers.size(), grainSize),
        [&](const tbb::blocked_range<int>& range) {
            for (size_t i = range.begin(); i < range.end(); ++i) {
                numbers[i]++;
            }
        },
        tbb::simple_partitioner());
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 3) {
        printf("usage: tbb_parallelForChunking <NUM_ELEMENTS> <GRAIN_SIZE>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeString<int>(argv[1]);
    int grainSize = DeserializeString<int>(argv[2]);

    // Run serial computation.
    std::vector<int> serialArray(numElements, 1);
    SerialForIncrement(serialArray);

    // Run parallel computation.
    std::vector<int> parallelArray(numElements, 1);
    ParallelForIncrement(grainSize, parallelArray);

    return EXIT_SUCCESS;
}
