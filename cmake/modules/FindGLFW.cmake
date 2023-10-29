
include(FetchContent)

find_package(glfw3 QUIET)
if (glfw3_FOUND)
  message(STATUS "Found a local GLFW3 installation (v. ${glfw3_VERSION})")
  set(GLFW_FOUND TRUE)
  set(GLFW_INCLUDE_DIR "${glfw3_INCLUDE_DIR}")
  set(GLFW_LIBRARIES glfw)
else()
  FetchContent_declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
  )
    FetchContent_GetProperties(glfw)
	if (NOT glfw_POPULATED)
	  FetchContent_Populate(glfw)
		option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
		option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
		option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
		option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
		
		add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})


		
	endif()
	set(GLFW_FOUND TRUE)
	set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
	set(GLFW_LIBRARIES ${glfw_LIBRARIES} glfw)
 
endif()