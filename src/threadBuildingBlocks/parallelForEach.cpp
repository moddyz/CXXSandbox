#include <tbb/parallel_for_each.h>
#include <tbb/atomic.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

static int
Fibonacci(int n)
{
    if (n >= 2) {
        return Fibonacci(n - 2) + Fibonacci(n - 1);
    } else {
        return n;
    }
}

static int
SerialForEach(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    int sum = 0;
    for (int value : array) {
        sum += Fibonacci(value);
    }

    return sum;
}

static int
ParallelForEach(std::vector<int>& array)
{
    PROFILE_FUNCTION();
    tbb::atomic<int> sum{ 0 };
    tbb::parallel_for_each(array.begin(), array.end(), [&](int value) {
        sum += Fibonacci(value);
    });

    return sum;
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelForEach <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run serial computation.
    std::vector<int> arrayA(numElements, 10);
    int serialSum = SerialForEach(arrayA);

    // Run parallel computation.
    std::vector<int> arrayB(numElements, 10);
    int parallelSum = ParallelForEach(arrayB);

    // Validate results.
    ASSERT(serialSum == parallelSum);

    return EXIT_SUCCESS;
}
