#include <tbb/concurrent_queue.h>
#include <tbb/parallel_for.h>

#include <queue>
#include <thread>

#include "utils.h"

using SerialQueueT = std::queue<int>;
using ConcurrentQueueT = tbb::concurrent_queue<int>;

static SerialQueueT SerialQueuePushAndPop(size_t numElements)
{
    PROFILE_FUNCTION();

    SerialQueueT queue;
    for (size_t i = 0; i < numElements; ++i) {
        queue.push(i);
    }

    for (size_t i = 0; i < numElements; ++i) {
        queue.pop();
    }

    ASSERT(queue.empty());

    return queue;
}

static ConcurrentQueueT ConcurrentQueuePushAndPop(size_t numElements)
{
    PROFILE_FUNCTION();

    ConcurrentQueueT queue;

    std::thread popThread([&]() {
        int count = 0;
        while (count != numElements) {
            int item;
            if (queue.try_pop(item)) {
                count++;
            }
        }
    });

    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              queue.push(i);
                          }
                      });
    popThread.join();

    ASSERT(queue.empty());

    return queue;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: tbb_parallelForLambda <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run both serial and concurrent maps on equal data.
    ASSERT(SerialQueuePushAndPop(numElements).empty());
    ASSERT(ConcurrentQueuePushAndPop(numElements).empty());

    return EXIT_SUCCESS;
}
