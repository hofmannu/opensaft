#include "Util/Timer.h"
#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

using namespace opensaft;
TEST_CASE("Timer: autostart") {
  Timer T;
  SECTION("Normal stop") {
    T.Stop();
    auto time = T.GetElapsedTime();
  }

  SECTION("Restart") { REQUIRE_THROWS(T.Start()); }

  SECTION("Restop") {
    T.Stop();
    REQUIRE_THROWS(T.Stop());
  }

  SECTION("Get elapsed before stop") { REQUIRE_THROWS(T.GetElapsedTime()); }
}