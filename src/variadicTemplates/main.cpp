#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <numeric>

template < typename First, typename... Args >
auto Sum( const First first, const Args... args ) -> decltype( first )
{
    const auto values = {first, args...};
    return std::accumulate( values.begin(), values.end(), First{0} );
}

template < typename First, typename... Args >
std::string StringConcatenate( const First first, const Args... args )
{
    auto concatenate = []( std::string i_lhs, decltype( first ) i_rhs ) { return std::move( i_lhs ) + ' ' + i_rhs; };

    const auto values = {first, args...};
    return std::accumulate( std::next( values.begin() ), values.end(), std::string( first ), concatenate );
}

TEST_CASE( "VariadicTemplates" )
{
    CHECK( Sum( 1, 2, 3 ) == 6 );
    CHECK( Sum( 2.0, 2.5, 3.0 ) == 7.5f );
    CHECK( StringConcatenate( "Foo", "Bar" ) == "Foo Bar" );
}
