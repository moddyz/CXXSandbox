#include <catch2/catch.hpp>

#include "vector.h"

static const char* s_templateProduct = "[template][product]";
using ElementTypes = std::tuple<int, float>;

TEMPLATE_PRODUCT_TEST_CASE("Vector_sizeOf",
                           s_templateProduct,
                           (std::vector, Vector),
                           ElementTypes)
{
    TestType vec;
    REQUIRE(sizeof(vec) > 0);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_defaultConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           ElementTypes)
{
    TestType vec;
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_resize",
                           s_templateProduct,
                           (std::vector, Vector),
                           ElementTypes)
{
    TestType vec;
    vec.resize(5);
    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() == 5);

    vec.resize(10);
    REQUIRE(vec.size() == 10);
    REQUIRE(vec.capacity() == 10);

    vec.resize(3);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 10);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_reserve",
                           s_templateProduct,
                           (std::vector, Vector),
                           ElementTypes)
{
    TestType vec;
    vec.reserve(5);
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 5);

    vec.reserve(10);
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 10);

    vec.reserve(5);
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 10);
}
