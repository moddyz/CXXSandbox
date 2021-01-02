#include <tbb/parallel_for.h>
#include <tbb/spin_mutex.h>
#include <tbb/mutex.h>
#include <tbb/queuing_mutex.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

template<typename MutexT>
static int
CounterProgram(size_t numElements)
{
    PROFILE_FUNCTION();

    int count = 0;
    MutexT mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          typename MutexT::scoped_lock lock(mutex);
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              count += 1;
                          }
                      });

    return count;
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_mutexComparison <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    ASSERT(CounterProgram<tbb::spin_mutex>(numElements) == numElements);
    ASSERT(CounterProgram<tbb::mutex>(numElements) == numElements);
    ASSERT(CounterProgram<tbb::queuing_mutex>(numElements) == numElements);

    return EXIT_SUCCESS;
}
