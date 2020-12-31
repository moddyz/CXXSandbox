#include <tbb/parallel_for.h>
#include <tbb/spin_mutex.h>
#include <tbb/mutex.h>
#include <tbb/queuing_mutex.h>

#include <stdio.h>
#include <vector>

#include "utils.h"

template<typename MutexT>
static void
CounterProgram(size_t numElements)
{
    PROFILE_FUNCTION();

    int counter = 0;
    MutexT mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              typename MutexT::scoped_lock lock(mutex);
                              counter += 1;
                          }
                      });
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

    CounterProgram<tbb::spin_mutex>(numElements);
    CounterProgram<tbb::mutex>(numElements);
    CounterProgram<tbb::queuing_mutex>(numElements);

    return EXIT_SUCCESS;
}
