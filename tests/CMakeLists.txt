find_package(Catch2 REQUIRED)

add_executable(UnitTests
	TestMain.cpp
	TestUltrasoundSignals.cpp
	TestVectorN.cpp
	TestVolume.cpp
	TestTimer.cpp
	TestSaft.cpp
	)

target_link_libraries(UnitTests
PRIVATE
	${catch2_LIBRARIES}
	opensaft
)

target_include_directories(UnitTests
	PUBLIC
	${catch2_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

install(
	TARGETS UnitTests
	RUNTIME DESTINATION bin
)