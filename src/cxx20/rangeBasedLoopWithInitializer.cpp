#include <catch2/catch.hpp>

TEST_CASE("rangeBasedLoopWithInitializer")
{
    std::vector<int> collector;
    for (std::vector vector{ 1, 2, 3 }; int element : vector) {
        collector.push_back(element);
    }

    CHECK(collector[0] == 1);
    CHECK(collector[1] == 2);
    CHECK(collector[2] == 3);
}
