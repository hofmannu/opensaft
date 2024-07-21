#include "Memory/UltrasoundSignals.h"
#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

using namespace opensaft;

TEST_CASE("TimeSignals: basic tests") {
  TimeSignal T(100);
  REQUIRE(T.size() == 100);
  for (const auto& elem : T)
    REQUIRE(elem == 0.0f);
}

TEST_CASE("TimeSignals: remove DC test") {
  constexpr std::size_t nt = 100;
  TimeSignal T(nt);

  // fill with sinosoidial shape
  for (std::size_t it = 0; it < nt; it++) {
    const float value = static_cast<float>(it) / static_cast<float>(nt) * M_PI;
    T[it] = value + 1.23f;
  }

  T.RemoveDC();

  double avg = 0.0;
  for (auto& elem : T) {
    avg += elem;
  }
  REQUIRE(std::abs(avg) < 1e-4);
}