cmake_minimum_required(VERSION 3.12...3.28)

# set project name
project(opensaft
	VERSION 1.0
	DESCRIPTION "Open source implementation of 3D SAFT"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(OPENSAFT_CUDA_SUPPORT "Enable CUDA support" OFF)

# prepare for cuda compilation
if (OPENSAFT_CUDA_SUPPORT)
	enable_language(CUDA)
	find_package(CUDA REQUIRED QUIET)
	include(FindCUDA)
	INCLUDE_DIRECTORIES(/opt/cuda/include include)
	set(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE OFF)
endif()

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_CUDA_FLAGS "${CUDA_CUDA_FLAGS} -rdc=true -Xptxas -v -O2")
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -x cu -rdc=true -ftz=false -prec-div=true -prec-sqrt=true -g)

# add path cmake/modules to search directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# find dependencies (most of them are handled through FetchContent)
find_package(ImGUI REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_library(H5CPP_LIB hdf5_cpp)
find_library(H5_LIB hdf5)
find_package(ImPlot REQUIRED)
find_package(ImGUIFileDialog REQUIRED)
find_package(CVolume REQUIRED)

add_library(opensaft STATIC "")
add_subdirectory(src/)

find_library(TERM_LIB curses)

# add an executable for the main graphical user interface
add_executable(main_exp src/main.cpp)
if (OPENSAFT_CUDA_SUPPORT)
	set_property(TARGET main_exp PROPERTY CUDA_SEPARABLE_COMPILATION ON)
	set_property(TARGET main_exp PROPERTY CUDA_ARCHITECTURES 50 75 86)
endif()
target_link_libraries(main_exp PUBLIC opensaft)

install(TARGETS main_exp DESTINATION bin)
