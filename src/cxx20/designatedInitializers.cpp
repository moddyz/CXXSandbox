#include <catch2/catch.hpp>

struct Foo
{
    int A;
    int B;
    int C = 5;
};

TEST_CASE("designatedInitializers")
{
    Foo foo{ .B = 6 };
    CHECK(foo.A == 0);
    CHECK(foo.B == 6);
    CHECK(foo.C == 5);
}
