#include <tbb/pipeline.h>

#include <stdio.h>
#include <vector>
#include <array>

#include "utils.h"

// Slice of an array.
constexpr int SLICE_SIZE = 100;
using Slice = std::array<int, SLICE_SIZE>;

static int
ProcessSlice(const Slice& slice)
{
    int sum = 0;
    for (size_t index = 0; index < slice.size(); ++index) {
        sum += slice[index];
    }
    return sum;
}

static std::vector<int>
SerialPipeline(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    std::vector<int> outputArray;
    for (size_t index = 0; index < array.size(); index += SLICE_SIZE) {
        // Data input.
        int begin = index;
        int end = index + SLICE_SIZE;
        if (index + SLICE_SIZE > array.size()) {
            end = array.size();
        }
        Slice* slice = new Slice();
        slice->fill(0);
        std::copy(array.begin() + begin, array.begin() + end, slice->begin());

        // Data transformation.
        int result = ProcessSlice(*slice);

        // Data output
        outputArray.push_back(result);

        // Clean up.
        delete slice;
    }

    return outputArray;
}

static std::vector<int>
ParallelPipeline(std::vector<int>& array)
{
    PROFILE_FUNCTION();

    // Serial input filter.
    int index = 0;
    tbb::filter_t<void, Slice*> inputFilter(
        tbb::filter::serial_in_order,
        [&](tbb::flow_control& flowControl) -> Slice* {
            if (index >= array.size()) {
                flowControl.stop();
                return nullptr;
            }

            // Data input.
            int begin = index;
            int end = index + SLICE_SIZE;
            if (index + SLICE_SIZE > array.size()) {
                end = array.size();
            }
            Slice* slice = new Slice();
            slice->fill(0);
            std::copy(
                array.begin() + begin, array.begin() + end, slice->begin());

            index += SLICE_SIZE;

            return slice;
        });

    // Parallel processing filter.
    tbb::filter_t<Slice*, int> processFilter(
        tbb::filter::parallel, [=](Slice* slice) {
            int result = ProcessSlice(*slice);
            delete slice;
            return result;
        });

    // Serial output filter.
    std::vector<int> outputArray;
    tbb::filter_t<int, void> outputFilter(
        tbb::filter::serial_in_order,
        [&](int result) { outputArray.push_back(result); });

    tbb::filter_t<void, void> filters =
        inputFilter & processFilter & outputFilter;

    // Configure level of parallelism, then execute pipeline.
    size_t parallelLevel = (array.size() + SLICE_SIZE - 1) / SLICE_SIZE;
    tbb::parallel_pipeline(parallelLevel, filters);

    return outputArray;
}

int
main(int argc, char** argv)
{
    // Parse arguments.
    if (argc != 2) {
        printf("usage: tbb_parallelPipeline <NUM_ELEMENTS>\n");
        return EXIT_FAILURE;
    }

    int numElements = DeserializeValue<int>(argv[1]);

    // Run serial pipeline.
    std::vector<int> arrayA(numElements);
    for (size_t i = 0; i < arrayA.size(); ++i) {
        arrayA[i] = i;
    }
    std::vector<int> outputA = SerialPipeline(arrayA);

    // Run parallel pipeline.
    std::vector<int> arrayB(numElements);
    for (size_t i = 0; i < arrayB.size(); ++i) {
        arrayB[i] = i;
    }
    std::vector<int> outputB = ParallelPipeline(arrayB);

    // Compare results.
    ASSERT(outputA.size() == outputB.size());
    for (size_t index = 0; index < outputA.size(); ++index) {
        ASSERT(outputA[index] == outputB[index]);
    }

    return EXIT_SUCCESS;
}
