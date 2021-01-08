#include <catch2/catch.hpp>

#include "vector.h"

static const char* s_templateProduct = "[template][product]";

//
// Construction
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_defaultConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec;
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);
}

//
// Capacity
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_reserve",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_shrink_to_fit",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec;
    vec.shrink_to_fit();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);

    vec.reserve(10);
    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 10);
    vec.shrink_to_fit();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);

    vec.resize(10);
    CHECK(vec.size() == 10);
    CHECK(vec.capacity() == 10);

    vec.clear();
    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 10);

    vec.shrink_to_fit();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);
}

//
// Modifiers
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_clear",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec;
    vec.clear();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);

    vec.reserve(10);
    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 10);
    vec.clear();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 10);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_push_back",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() > 5);

    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(vec[i] == typename TestType::value_type(i + 1));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_resize",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec;
    vec.resize(5);
    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() == 5);
    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(vec[i] == typename TestType::value_type());
    }

    vec.resize(10);
    REQUIRE(vec.size() == 10);
    REQUIRE(vec.capacity() == 10);
    for (size_t i = 0; i < 10; ++i) {
        REQUIRE(vec[i] == typename TestType::value_type());
    }

    vec.resize(3);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() == 10);
    for (size_t i = 0; i < 3; ++i) {
        REQUIRE(vec[i] == typename TestType::value_type());
    }
}
