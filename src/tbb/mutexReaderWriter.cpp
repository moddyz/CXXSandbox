#include <tbb/spin_rw_mutex.h>
#include <tbb/null_rw_mutex.h>
#include <tbb/queuing_rw_mutex.h>
#include <tbb/parallel_for.h>

#include <unordered_map>
#include "utils.h"

using ContainerT = std::unordered_map<int, std::string>;

std::string ComputeValueForKey(int key)
{
    int remainder = key % 23;
    return SerializeValue(remainder);
}

template<typename ReaderWriterMutexT>
const std::string& ThreadSafeGetOrCreate(int key,
                                         ContainerT& container,
                                         ReaderWriterMutexT& mutex)
{
    // Acquire reader lock.
    typename ReaderWriterMutexT::scoped_lock lock(mutex, /* is_writer */ false);

    // If key already exists, return its value.
    ContainerT::const_iterator it = container.find(key);
    if (it != container.end()) {
        return it->second;
    }

    // Otherwise, upgrade to writer lock and create the key.
    // upgrade_to_writer may have to temporarily release the lock before
    // upgrading this to the writer. (XXX: What is this particular scenario?)
    // upgrade_to_writer will return true if it managed to upgrade _without_
    // releasing temporarily.
    if (!lock.upgrade_to_writer()) {
        // Did temporarily release lock, thus we must try to find it again
        // incase it was inserted during release.
        ContainerT::const_iterator it = container.find(key);
        if (it != container.end()) {
            return it->second;
        }
    }

    std::pair<ContainerT::iterator, bool> insertion =
        container.insert(std::make_pair(key, ComputeValueForKey(key)));
    ASSERT(insertion.second);

    return insertion.first->second;
}

template<typename ReaderWriterMutexT>
static void InsertProgram(size_t numElements)
{
    PROFILE_FUNCTION();

    ContainerT container;
    ReaderWriterMutexT mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              const std::string& value =
                                  ThreadSafeGetOrCreate(i, container, mutex);
                              ASSERT(value == ComputeValueForKey(i));
                          }
                      });
}

int main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_mutexComparison <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Try different mutex types.
    InsertProgram<tbb::spin_rw_mutex>(numElements);
    InsertProgram<tbb::queuing_rw_mutex>(numElements);

    // XXX: Why does this hang?
    // InsertProgram<tbb::null_rw_mutex>(numElements);

    return EXIT_SUCCESS;
}
