#include "Memory/Volume.h"
#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

using namespace opensaft;

TEST_CASE("Volume: checking constructors") {
  Volume V({100, 100, 100});
  for (const auto& elem : V)
    REQUIRE(elem == 0.0f);

  REQUIRE(V.size() == 100 * 100 * 100);
}

TEST_CASE("Volume: checking access operators") {
  const std::size_t iy = GENERATE(1, 2, 0, 3);
  Volume V({10, 10, 10});
  V(0, iy, 0) = 10.0f;
  REQUIRE(V(0, iy, 0) == 10.0f);
  REQUIRE(V[iy * 10] == 10.0f);

  // make sure all other elements are still 0.0f
  for (std::size_t idx = 0; idx < 1000; idx++) {
    if (idx != iy * 10) {
      REQUIRE(V[idx] == 0.0f);
    }
  }
}

TEST_CASE("Volume: assign to full volume") {
  Volume V({2, 2, 2});
  const float setPoint = GENERATE(-1.0f, 2.0f, 3.12f);
  V = setPoint;
  for (const auto& elem : V) {
    REQUIRE(elem == setPoint);
  }
}