#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE( "LambdaExpressions" )
{
    int x = 1;

    auto GetX    = [=] { return x; };
    auto GetXRef = [&] { return x; };

    x++;

    CHECK( GetX() == 1 );
    CHECK( GetXRef() == 2 );
}
