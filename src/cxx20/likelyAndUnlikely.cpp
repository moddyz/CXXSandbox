#include <catch2/catch.hpp>

TEST_CASE("likelyAndUnlikely")
{
    int random = 5;
    [[likely]] if (random < 6) { CHECK(random == 5); }
}
