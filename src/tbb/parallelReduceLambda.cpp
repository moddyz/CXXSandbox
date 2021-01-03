#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

static int
SerialReduce(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    int sum = 0;
    for (size_t i = 0; i < array.size(); ++i) {
        sum += array[i];
    }

    return sum;
}

static int
ParallelReduce(std::vector<int>& array)
{
    PROFILE_FUNCTION();
    // XXX: Aren't the kernel and reduction function the same?
    return tbb::parallel_reduce(
        /* range */ tbb::blocked_range<int>(0, array.size()),
        /* identity */ 0,
        /* kernel */
        [&](tbb::blocked_range<int> range, int sum) {
            for (int i = range.begin(); i < range.end(); ++i) {
                sum += array[i];
            }
            return sum;
        },
        /* reduction */ std::plus<int>());
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelReduceLambda <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run serial computation.
    std::vector<int> arrayA(numElements, 1);
    int serialResult = SerialReduce(arrayA);

    // Run parallel computation.
    std::vector<int> arrayB(numElements, 1);
    int parallelResult = ParallelReduce(arrayB);

    ASSERT(serialResult == parallelResult);

    return EXIT_SUCCESS;
}
