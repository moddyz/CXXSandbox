#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <cppExperiments/inlineNamespaces/api.h>

TEST_CASE( "InlineNamespaces" )
{
    CHECK( library::GetVersion() == 2 );
    CHECK( library::v1::GetVersion() == 1 );
    CHECK( library::v2::GetVersion() == 2 );
}
