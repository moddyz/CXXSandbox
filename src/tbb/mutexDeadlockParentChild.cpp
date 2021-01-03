#include <tbb/mutex.h>

#include <thread>

#include "utils.h"

template<typename MutexT>
static void
ChildFunction(MutexT& mutex)
{
    typename MutexT::scoped_lock lock(mutex);
    printf("I'm a child.");
}

template<typename MutexT>
static void
ParentFunction(MutexT& mutex)
{
    typename MutexT::scoped_lock lock(mutex);
    std::thread childThread(ChildFunction<MutexT>, std::ref(mutex));
    childThread.join();
}

int
main(int argc, char** argv)
{
    tbb::mutex mutex;
    std::thread thread(ParentFunction<decltype(mutex)>, std::ref(mutex));
    thread.join();

    return EXIT_SUCCESS;
}
