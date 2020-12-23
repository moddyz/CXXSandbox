#include <tbb/parallel_for.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

static int
Computation(int a, int b, int c)
{
    return a * b / (c + 1);
}

static void
SerialFor(std::vector<int>& array)
{
    PROFILE_FUNCTION();
    for (size_t i = 0; i < array.size(); ++i) {
        array[i] = Computation(array[i], array[i], i);
    }
}

static void
ParallelFor(int grainSize, std::vector<int>& array)
{
    PROFILE_FUNCTION();
    tbb::parallel_for(
        tbb::blocked_range<int>(0, array.size(), grainSize),
        [&](const tbb::blocked_range<int>& range) {
            for (size_t i = range.begin(); i < range.end(); ++i) {
                array[i] = Computation(array[i], array[i], i);
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
    SerialFor(serialArray);

    // Run parallel computation.
    std::vector<int> parallelArray(numElements, 1);
    ParallelFor(grainSize, parallelArray);

    return EXIT_SUCCESS;
}
