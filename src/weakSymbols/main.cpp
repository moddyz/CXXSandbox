#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

extern std::string
GetString() __attribute__((weak));

TEST_CASE("weakSymbols")
{
    CHECK(GetString() == "Strong");
}
