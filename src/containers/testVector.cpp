#include <catch2/catch.hpp>

#include "vector.h"

static const char* s_templateProduct = "[template][product]";

//
// Construction
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_DefaultConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
{
    TestType vec;
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() == 0);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_CountConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
{
    TestType vec(5);
    REQUIRE(vec.size() == 5);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_DefaultValueConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    TestType vec(5, 5);
    REQUIRE(vec.size() == 5);
    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(vec[i] == 5);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_DefaultValueConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec(5, "foo");
    REQUIRE(vec.size() == 5);
    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(vec[i] == typename TestType::value_type("foo"));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_CopyConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vecA;
    vecA.push_back("foo");
    vecA.push_back("bar");

    TestType vecB(vecA);
    REQUIRE(vecB.size() == vecA.size());
    REQUIRE(vecB.capacity() == vecA.capacity());
    REQUIRE(vecB[0] == typename TestType::value_type("foo"));
    REQUIRE(vecB[1] == typename TestType::value_type("bar"));
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_CopyAssignmentConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vecA;
    vecA.push_back("foo");
    vecA.push_back("bar");

    TestType vecB = vecA;
    REQUIRE(vecB.size() == vecA.size());
    REQUIRE(vecB.capacity() == vecA.capacity());
    REQUIRE(vecB[0] == typename TestType::value_type("foo"));
    REQUIRE(vecB[1] == typename TestType::value_type("bar"));
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_InitializerListConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec = {
        std::string("foo"),
        std::string("bar"),
        std::string("baz"),
        std::string("qux"),
    };

    REQUIRE(vec.size() == 4);
    REQUIRE(vec[0] == typename TestType::value_type("foo"));
    REQUIRE(vec[1] == typename TestType::value_type("bar"));
    REQUIRE(vec[2] == typename TestType::value_type("baz"));
    REQUIRE(vec[3] == typename TestType::value_type("qux"));
}

//
// Element access
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_AssignmentOperator",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec.resize(2);
    vec[0] = "foo";
    vec[1] = "bar";
    REQUIRE(vec[0] == typename TestType::value_type("foo"));
    REQUIRE(vec[1] == typename TestType::value_type("bar"));
}

//
// Capacity
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_reserve",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
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
                           (int, float, std::string))
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
                           (int, float, std::string))
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_push_back",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec.push_back("Foo");
    vec.push_back("Bar");
    vec.push_back("Baz");

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() > 3);

    REQUIRE(vec[0] == typename TestType::value_type("Foo"));
    REQUIRE(vec[1] == typename TestType::value_type("Bar"));
    REQUIRE(vec[2] == typename TestType::value_type("Baz"));
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_resize",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_swap",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float))
{
    // Create vecA.
    TestType vecA;
    vecA.push_back(1);
    vecA.push_back(2);
    vecA.push_back(3);
    size_t originalCapacityA = vecA.capacity();

    // Create vecB.
    TestType vecB;
    vecB.push_back(4);
    vecB.push_back(5);
    size_t originalCapacityB = vecB.capacity();

    // Swap the vectors.
    vecA.swap(vecB);

    // Check vecA members.
    REQUIRE(vecA.size() == 2);
    REQUIRE(vecA.capacity() == originalCapacityB);
    REQUIRE(vecA[0] == 4);
    REQUIRE(vecA[1] == 5);

    // Check vecB members.
    REQUIRE(vecB.size() == 3);
    REQUIRE(vecB.capacity() == originalCapacityA);
    REQUIRE(vecB[0] == 1);
    REQUIRE(vecB[1] == 2);
    REQUIRE(vecB[2] == 3);
}
