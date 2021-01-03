#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

static int Computation(int a, int b, int c)
{
    return a * b / (c + 1);
}

static int SerialReduce(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    int sum = 0;
    for (size_t i = 0; i < array.size(); ++i) {
        sum += Computation(array[i], array[i], i);
    }

    return sum;
}

class Reduction
{
public:
    Reduction(std::vector<int>& array)
      : m_array(array)
    {}

    // "Splitting" constructor as required by parallel_reduce.
    // The dummy tbb::split argument distinguishes this ctor from
    // a copy constructor.
    Reduction(Reduction& other, tbb::split)
      : m_array(other.m_array)
    {}

    // Join another reduced subsum with this one.
    void join(const Reduction& other) { m_sum += other.m_sum; }

    void operator()(const tbb::blocked_range<int>& range)
    {
        // Same class instance might be used for mutiple sub-ranges, so we
        // cannot discard previous accumulated results;
        int sum = m_sum;
        for (size_t i = range.begin(); i < range.end(); ++i) {
            sum += Computation(m_array[i], m_array[i], i);
        }

        m_sum = sum;
    }

    int GetSum() const { return m_sum; }

private:
    const std::vector<int>& m_array;
    int m_sum = 0;
};

static int ParallelReduce(std::vector<int>& array)
{
    PROFILE_FUNCTION();
    Reduction reduction(array);
    tbb::parallel_reduce(tbb::blocked_range<int>(0, array.size()), reduction);
    return reduction.GetSum();
}

int main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelReduce <NUM_ELEMENTS>\n");
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
