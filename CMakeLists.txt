cmake_minimum_required(VERSION 3.12...3.28)

# set project name
project(opensaft
	VERSION 1.0
	DESCRIPTION "Open source implementation of 3D SAFT"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")


option(OPENSAFT_CUDA_SUPPORT "Enable CUDA support" OFF)
option(OPENSAFT_TESTING "Build unit tests" ON)

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
find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(H5CPP_LIB hdf5_cpp)
find_library(H5_LIB hdf5) # todo this sounds fishy that
find_package(ImPlot REQUIRED)
find_package(ImGUIFileDialog REQUIRED)
find_package(Backward REQUIRED)

# main library holding the backbone of the application
add_library(opensaft STATIC "")

add_subdirectory(src/)

target_link_libraries(opensaft PUBLIC
	${ImGUI_LIBRARIES}
	${implot_LIBRARIES}
	${GLFW_LIBRARIES}
	${cvolume_LIBRARIES}
	${ImGUIFileDialog_LIBRARIES}
	${backward_LIBRARIES}
)

target_include_directories(opensaft PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${ImGUI_INCLUDE_DIR}
	${ImPlot_INCLUDE_DIR}
	${ImGUIFileDialog_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
	${GLFW3_INCLUDE_DIR}
	${cvolume_INCLUDE_DIR}
	${backward_INCLUDE_DIR}
)


find_library(TERM_LIB curses)

if (OPENSAFT_TESTING)
	add_subdirectory(tests)
endif()

# add an executable for the main graphical user interface
add_executable(main_exp src/main.cpp)
if (OPENSAFT_CUDA_SUPPORT)
	set_property(TARGET main_exp PROPERTY CUDA_SEPARABLE_COMPILATION ON)
	set_property(TARGET main_exp PROPERTY CUDA_ARCHITECTURES 50 75 86)
endif()
target_link_libraries(main_exp PUBLIC opensaft)

install(TARGETS main_exp DESTINATION bin)
