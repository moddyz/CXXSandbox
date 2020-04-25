#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

namespace library
{
namespace v1
{
int GetVersion()
{
    return 1;
};

} // namespace v1
inline namespace v2
{
int GetVersion()
{
    return 2;
};

} // namespace v2
} // namespace library

TEST_CASE( "InlineNamespaces" )
{
    CHECK( library::GetVersion() == 2 );
    CHECK( library::v1::GetVersion() == 1 );
    CHECK( library::v2::GetVersion() == 2 );
}
