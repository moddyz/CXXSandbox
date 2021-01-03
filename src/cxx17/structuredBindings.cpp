// Structured bindings in C++17.
//
// Initialize multiple variables from a tuple, pair, or struct concisely.

#include <catch2/catch.hpp>
#include <unordered_map>

using Vec3f = std::tuple< float, float, float >;

Vec3f GetZeroVector()
{
    return Vec3f{0.0f, 0.0f, 0.0f};
}

TEST_CASE( "structuredBindings" )
{
    // De-composition of tuples.
    const auto [ x, y, z ] = GetZeroVector();
    CHECK( x == 0.0f );
    CHECK( y == 0.0f );
    CHECK( z == 0.0f );

    // De-composition of map items.
    std::unordered_map< std::string, int > mapping{{"a", 1}, {"b", 2}, {"c", 3}};
    for ( const auto& [ key, value ] : mapping )
    {
        printf( "%s: %d\n", key.c_str(), value );
    }
}
