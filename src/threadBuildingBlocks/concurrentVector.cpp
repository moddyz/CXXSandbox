#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

#include <string>
#include <vector>

#include "utils.h"

using SerialVectorT = std::vector<int>;
using ConcurrentVectorT = tbb::concurrent_vector<int>;

static SerialVectorT
SerialVectorPushBack(size_t numElements)
{
    PROFILE_FUNCTION();

    SerialVectorT vector;
    for (size_t i = 0; i < numElements; ++i) {
        vector.push_back(i);
    }

    return vector;
}

static ConcurrentVectorT
ConcurrentVectorPushBack(size_t numElements)
{
    PROFILE_FUNCTION();

    ConcurrentVectorT vector;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              vector.push_back(i);
                          }
                      });

    return vector;
}

static ConcurrentVectorT
ConcurrentVectorGrowToAtLeast(size_t numElements)
{
    PROFILE_FUNCTION();

    ConcurrentVectorT vector;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              vector.grow_to_at_least(numElements);
                              vector[i] = i;
                          }
                      });

    return vector;
}

int
main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: tbb_parallelForLambda <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run both serial and concurrent maps on equal data.
    ASSERT(SerialVectorPushBack(numElements).size() == numElements);
    ASSERT(ConcurrentVectorPushBack(numElements).size() == numElements);
    ASSERT(ConcurrentVectorGrowToAtLeast(numElements).size() == numElements);

    return EXIT_SUCCESS;
}
