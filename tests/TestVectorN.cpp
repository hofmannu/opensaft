
#include <catch2/catch_all.hpp>
#include "VectorN.h"

using namespace opensaft;

TEST_CASE("VectorN: testing constructors") {
	// empty constructor
	{
		VectorN<float, 4> vec1;
		REQUIRE(vec1[0] == 0.0f);
		REQUIRE(vec1[1] == 0.0f);
		REQUIRE(vec1[2] == 0.0f);
		REQUIRE(vec1[3] == 0.0f);
	}

	// constructing all values to single float
	{
		float initValue = GENERATE(10.0f, -20.0f, 2.0f);
		VectorN<float, 4> vec2(initValue);
		REQUIRE(vec2[0] == initValue);
		REQUIRE(vec2[1] == initValue);
		REQUIRE(vec2[2] == initValue);
		REQUIRE(vec2[3] == initValue);
	}
}