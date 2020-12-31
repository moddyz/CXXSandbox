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
ParallelFor(std::vector<int>& array)
{
    PROFILE_FUNCTION();
    tbb::parallel_for(tbb::blocked_range<int>(0, array.size()),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              array[i] = Computation(array[i], array[i], i);
                          }
                      });
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelForLambda <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run serial computation.
    std::vector<int> arrayA(numElements, 1);
    SerialFor(arrayA);

    // Run parallel computation.
    std::vector<int> arrayB(numElements, 1);
    ParallelFor(arrayB);

    return EXIT_SUCCESS;
}
