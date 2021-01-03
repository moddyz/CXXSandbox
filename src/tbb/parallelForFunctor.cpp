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

class Functor
{
public:
    Functor(std::vector<int>& array)
      : m_array(array)
    {}

    void operator()(const tbb::blocked_range<int>& range) const
    {
        for (size_t i = range.begin(); i != range.end(); ++i) {
            m_array[i] = Computation(m_array[i], m_array[i], i);
        }
    }

private:
    std::vector<int>& m_array;
};

static void
ParallelFor(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    tbb::parallel_for(tbb::blocked_range<int>(0, array.size()), Functor(array));
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelForFunctor <NUM_ELEMENTS>\n");
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
