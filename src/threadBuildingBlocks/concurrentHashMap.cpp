#include <tbb/concurrent_hash_map.h>
#include <tbb/parallel_for.h>

#include <string>
#include <unordered_map>

#include "utils.h"

using SerialHashMapT = std::unordered_map<std::string, std::string>;
using ConcurrentHashMapT = tbb::concurrent_hash_map<std::string, std::string>;

template<typename HashMapT>
static void
InsertValue(int value, HashMapT& hashMap)
{
    hashMap.insert(std::pair<std::string, std::string>(
        SerializeValue(value), SerializeValue(value * 10)));
}

static SerialHashMapT
SerialHashMap(size_t numElements)
{
    PROFILE_FUNCTION();

    SerialHashMapT hashMap;
    for (size_t i = 0; i < numElements; ++i) {
        InsertValue(i, hashMap);
    }

    return hashMap;
}

static ConcurrentHashMapT
ConcurrentHashMap(size_t numElements)
{
    PROFILE_FUNCTION();

    tbb::concurrent_hash_map<std::string, std::string> hashMap;
    tbb::parallel_for(tbb::blocked_range<int>(0, numElements),
                      [&](const tbb::blocked_range<int>& range) {
                          for (size_t i = range.begin(); i < range.end(); ++i) {
                              InsertValue(i, hashMap);
                          }
                      });

    return hashMap;
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
    SerialHashMapT serialHashMap = SerialHashMap(numElements);
    ConcurrentHashMapT concurrentHashMap = ConcurrentHashMap(numElements);

    // Validate results.
    ASSERT(serialHashMap.size() == numElements);
    ASSERT(concurrentHashMap.size() == numElements);
    ConcurrentHashMapT::const_accessor constAccessor;
    for (const SerialHashMapT::value_type& item : serialHashMap) {
        ASSERT(concurrentHashMap.find(constAccessor, item.first));
        ASSERT(item.second == constAccessor->second);
    }

    return EXIT_SUCCESS;
}
