#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "vector.h"

TEMPLATE_PRODUCT_TEST_CASE("Vector_Constructor",
                           "[template][product]",
                           (std::vector, Vector),
                           (int, float))
{
    BENCHMARK("Benchmark")
    {
        TestType vec;
        return sizeof(vec);
    };
}
