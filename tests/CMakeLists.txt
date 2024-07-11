find_package(Catch2 REQUIRED)

add_executable(UnitTests
	test_main.cpp
	TestVectorN.cpp
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