# little cmake module to get hands on catch2
# todo add option to use global installation candidate
include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
  FetchContent_Populate(catch2)
  add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
endif()

set(catch2_INCLUDE_DIR "${catch2_SOURCE_DIR}/;${catch2_SOURCE_DIR}/src/;" CACHE PATH "Path to include folder for Catch2")
set(catch2_LIBRARIES "Catch2::Catch2WithMain" CACHE STRING "Library for Catch2")
set(catch2_FOUND TRUE CACHE BOOL "Catch2 found")
