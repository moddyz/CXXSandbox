// Inline Namespaces in C++ 11
//
// Everything declared inside a inline namespace is _also_ part of the parent
// namespace.

#include <catch2/catch.hpp>

namespace library {
namespace v1 {
int GetVersion()
{
    return 1;
};

} // namespace v1
inline namespace v2 {
int GetVersion()
{
    return 2;
};

} // namespace v2
} // namespace library

TEST_CASE("InlineNamespaces")
{
    CHECK(library::GetVersion() == 2);
    CHECK(library::v1::GetVersion() == 1);
    CHECK(library::v2::GetVersion() == 2);
}
