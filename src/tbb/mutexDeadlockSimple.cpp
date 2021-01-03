#include <tbb/mutex.h>

int
main(int argc, char** argv)
{
    tbb::mutex mutex;
    tbb::mutex::scoped_lock lockA(mutex);
    tbb::mutex::scoped_lock lockB(mutex);
    return EXIT_SUCCESS;
}
