#include <tbb/parallel_invoke.h>

#include <vector>

#include "utils.h"

using ArrayT = std::vector<int>;

static int
Computation(int a, int b, int c)
{
    return a * b / (c + 1);
}

static ArrayT
SerialInvoke(int numElements)
{
    PROFILE_FUNCTION();

    ArrayT array(numElements, 1);
    for (size_t i = 0; i < numElements; ++i) {
        array[i] = Computation(i, i, i);
    }

    return array;
}

static ArrayT
ParallelInvoke(int numElements)
{
    PROFILE_FUNCTION();

    ArrayT array(numElements, 1);

    tbb::parallel_invoke(
        [&]() {
            for (size_t i = 0; i < numElements / 2; ++i) {
                array[i] = Computation(i, i, i);
            }
        },
        [&]() {
            for (size_t i = numElements / 2; i < numElements; ++i) {
                array[i] = Computation(i, i, i);
            }
        });

    return array;
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelInvoke <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    ArrayT serialArray = SerialInvoke(numElements);
    ArrayT parallelArray = ParallelInvoke(numElements);

    ASSERT(serialArray.size() == numElements);
    ASSERT(parallelArray.size() == numElements);

    for (size_t i = 0; i < serialArray.size(); ++i) {
        ASSERT(serialArray[i] == parallelArray[i]);
    }

    return EXIT_SUCCESS;
}
