#include <catch2/catch.hpp>

#include "vector.h"

static const char* s_templateProduct = "[template][product]";

// Test type.
struct Vec3f
{
    Vec3f() = default;

    explicit Vec3f(float _x, float _y, float _z)
      : x(_x)
      , y(_y)
      , z(_z)
    {}

    bool operator==(const Vec3f& other) const
    {
        return other.x == x and other.y == y and other.z == z;
    }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

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

TEMPLATE_PRODUCT_TEST_CASE("Vector_SizeConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
{
    TestType vec(5);
    REQUIRE(vec.size() == 5);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_SizeAndDefaultValueConstructor",
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_SizeAndDefaultValueConstructor",
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_InitializerListConstructor",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec{
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_CopyAssignmentOperator",
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_InitializerListAssignmentOperator",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec = {
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

TEMPLATE_PRODUCT_TEST_CASE("Vector_assign",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int))
{
    TestType vec;
    vec.resize(5, 1);
    CHECK(vec.size() == 5);
    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(vec[i] == 1);
    }

    vec.assign(3, 2);
    CHECK(vec.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
        REQUIRE(vec[i] == 2);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_assign_InitializerList",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec.assign({
        std::string("foo"),
        std::string("bar"),
        std::string("baz"),
        std::string("qux"),
    });

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

TEMPLATE_PRODUCT_TEST_CASE("Vector_at",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec.resize(2);
    vec.at(0) = "foo";
    vec.at(1) = "bar";
    REQUIRE(vec.at(0) == typename TestType::value_type("foo"));
    REQUIRE(vec.at(1) == typename TestType::value_type("bar"));

    REQUIRE_THROWS_AS(vec.at(2), std::out_of_range);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_front",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;

    vec.push_back("foo");
    REQUIRE(vec.front() == "foo");

    vec.push_back("bar");
    REQUIRE(vec.front() == "foo");
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_back",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;

    vec.push_back("foo");
    REQUIRE(vec.back() == "foo");

    vec.push_back("bar");
    REQUIRE(vec.back() == "bar");
}

//
// Iteration
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_begin",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vecA;
    vecA.push_back("foo");
    vecA.push_back("bar");
    vecA.push_back("baz");
    REQUIRE(*vecA.begin() == "foo");
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_iterator_IncrementForwards",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vecA;
    vecA.push_back("foo");
    vecA.push_back("bar");
    vecA.push_back("baz");

    typename TestType::iterator it = vecA.begin();
    it++;
    REQUIRE(*it == "bar");
    it++;
    REQUIRE(*it == "baz");
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_iterator_IncrementBackwards",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vecA;
    vecA.push_back("foo");
    vecA.push_back("bar");
    vecA.push_back("baz");

    typename TestType::iterator it = vecA.end();
    it--;
    REQUIRE(*it == "baz");
    it--;
    REQUIRE(*it == "bar");
    it--;
    REQUIRE(*it == "foo");
}

//
// Capacity
//

TEMPLATE_PRODUCT_TEST_CASE("Vector_empty",
                           s_templateProduct,
                           (std::vector, Vector),
                           (int, float, std::string))
{
    TestType vec;
    REQUIRE(vec.empty());
    vec.reserve(4);
    REQUIRE(vec.empty());
    vec.resize(2);
    REQUIRE(!vec.empty());
}

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
    vec.push_back(std::string("Foo"));
    vec.push_back(std::string("Bar"));
    vec.push_back(std::string("Baz"));

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() > 3);

    REQUIRE(vec[0] == typename TestType::value_type("Foo"));
    REQUIRE(vec[1] == typename TestType::value_type("Bar"));
    REQUIRE(vec[2] == typename TestType::value_type("Baz"));
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_emplace_back",
                           s_templateProduct,
                           (std::vector, Vector),
                           (Vec3f))
{
    TestType vec;
    typename TestType::value_type element0 = vec.emplace_back(1, 2, 3);
    typename TestType::value_type element1 = vec.emplace_back(2, 3, 4);

    REQUIRE(vec.size() == 2);

    REQUIRE(vec[0] == typename TestType::value_type(1, 2, 3));
    REQUIRE(vec[1] == typename TestType::value_type(2, 3, 4));
    REQUIRE(element0 == vec[0]);
    REQUIRE(element1 == vec[1]);
}

TEMPLATE_PRODUCT_TEST_CASE("Vector_pop_back",
                           s_templateProduct,
                           (std::vector, Vector),
                           (std::string))
{
    TestType vec;
    vec.push_back(std::string("Foo"));
    vec.push_back(std::string("Bar"));
    vec.push_back(std::string("Baz"));

    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() > 3);

    vec.pop_back();
    REQUIRE(vec.size() == 2);
    REQUIRE(vec.capacity() > 3);
    REQUIRE(vec[0] == typename TestType::value_type("Foo"));
    REQUIRE(vec[1] == typename TestType::value_type("Bar"));

    vec.pop_back();
    REQUIRE(vec.size() == 1);
    REQUIRE(vec.capacity() > 3);
    REQUIRE(vec[0] == typename TestType::value_type("Foo"));

    vec.pop_back();
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() > 3);
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
