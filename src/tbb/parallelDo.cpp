#include <tbb/parallel_do.h>
#include <tbb/blocked_range.h>

#include <stdio.h>
#include <list>

#include "utils.h"

// TBB documentation says that each unit of work should take a least a few
// thousand instructions to reap benefits of processing a linked list (due to
// its serialized fetching of work).
static int RecursiveComputation(int a, int b, int c, int depth)
{
    if (depth == 0) {
        return 1;
    }

    return a * b / (c + 1) * RecursiveComputation(a, b, c, depth - 1);
}

static void SerialDo(std::list<int>& list)
{
    PROFILE_FUNCTION();
    for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it) {
        int& number = *it;
        number = RecursiveComputation(number, number, number, 500);
    }
}

static void ParallelDo(std::list<int>& list)
{
    PROFILE_FUNCTION();
    tbb::parallel_do(list.begin(), list.end(), [=](int& number) {
        number = RecursiveComputation(number, number, number, 500);
    });
}

int main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelDo <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run serial computation.
    std::list<int> listA(numElements, 1);
    SerialDo(listA);

    // Run parallel computation.
    std::list<int> listB(numElements, 1);
    ParallelDo(listB);

    return EXIT_SUCCESS;
}
