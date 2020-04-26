#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Lambda Expressions since C++11
//
// Callable objects which can capture values or references at time of initialization.

TEST_CASE( "LambdaExpressions" )
{
    int x = 1;

    auto GetX    = [=] { return x; };
    auto GetXRef = [&] { return x; };

    x++;

    // GetX captured x by value was captured before it was incrememented.
    CHECK( GetX() == 1 );

    // GetX captured x by reference, so it assumes the most up-to-date value of x.
    CHECK( GetXRef() == 2 );
}
