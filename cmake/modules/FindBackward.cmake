include(FetchContent)

FetchContent_Declare(
  backward
  GIT_REPOSITORY https://github.com/bombela/backward-cpp
  GIT_TAG master  # or a version tag, such as v1.6
  SYSTEM          # optional, the Backward include directory will be treated as system directory
)

FetchContent_GetProperties(backward)
if (NOT backward_POPULATED)
  FetchContent_Populate(backward)
  add_subdirectory(${backward_SOURCE_DIR} ${backward_BINARY_DIR})
endif()

set(backward_INCLUDE_DIR "${backward_SOURCE_DIR}" CACHE PATH "Path to include folder for Backward")
set(backward_LIBRARIES "Backward::Interface" CACHE STRING "Library for Backward")
set(backward_FOUND TRUE CACHE BOOL "Backward found")
