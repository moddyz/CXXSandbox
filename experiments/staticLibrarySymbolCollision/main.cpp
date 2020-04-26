#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

namespace A
{
std::string FindLibrary();
} // namespace A

namespace B
{
std::string FindLibrary();
} // namespace B

// sharedLibA links staticLibA.
// sharedLibB links staticLibB.
//
// Each static library defines the GetLibraryName() symbol, which sharedLibA and sharedLibB call, respectively,
// in their FindLibrary implementations.
//
// B::FindLibrary() would return "staticLibB" if this test program does _not_ link sharedLibA.
TEST_CASE( "staticLibrarySymbolCollision" )
{
    CHECK( A::FindLibrary() == "staticLibA" );
    CHECK( B::FindLibrary() == "staticLibA" );
}
