#include "Memory/UltrasoundSignals.h"
#include "Saft.h"
#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

using namespace opensaft;

TEST_CASE("Saft: simple test recon") {

  UltrasoundSignals sig;

  Saft S;
  S.SetInput(std::move(sig));
  S.Launch();
  S.Wait();
}