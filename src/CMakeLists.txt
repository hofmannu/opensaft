set(OpenGL_GL_PREFERENCE GLVND)
cmake_policy(SET CMP0079 NEW)

# prepare for cuda compilation
enable_language(CUDA)
find_package(CUDA REQUIRED QUIET)
INCLUDE(FindCUDA)
INCLUDE_DIRECTORIES(/opt/cuda/include include)
set(CUDA_SEPARABLE_COMPILATION ON)
set(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE OFF)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CUDA_FLAGS "${CUDA_CUDA_FLAGS} -rdc=true -ftz=false -prec-div=true -prec-sqrt=true")
set(CMAKE_CUDA_FLAGS "${CUDA_CUDA_FLAGS} -rdc=true --ptxas-options=-v -O2")


find_library(H5CPP_LIB hdf5_cpp)
find_library(H5_LIB hdf5)


add_library(Interface interface.cpp)
set_property(TARGET Interface PROPERTY CUDA_SEPARABLE_COMPILATION ON)
set_property(TARGET Interface PROPERTY CUDA_ARCHITECTURES 50 75 86)
target_link_libraries(Interface PUBLIC
	ImGuiPlot 
	GLEW 
	ImGuiImplSdl 
	ImGuiImplOpenGL3
	ImGuiFileDialog
	Saft
	Transducer
	ReconSettings
	ColorMapper
)

add_library(Saft saft.cu)
target_link_libraries(Saft PUBLIC 
	Transducer
	ReconSettings
	Volume)

add_library(Transducer transducer.cpp)
add_library(ReconSettings reconSettings.cpp)

add_library(ImGuiFileDialog ImGuiFileDialog.cpp)
target_link_libraries(ImGuiFileDialog PUBLIC ImGui)

add_library(ImGuiPlot imgui_plot.cpp)

add_library(ImGuiImplOpenGL3 imgui_impl_opengl3.cpp)
target_link_libraries(ImGuiImplOpenGL3 PUBLIC ImGui OpenGL)

find_package(SDL2 REQUIRED)
add_library(ImGuiImplSdl imgui_impl_sdl.cpp)
target_link_libraries(ImGuiImplSdl PUBLIC ImGui SDL2 OpenGL)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})
link_libraries(${GLFW_LIBRARY_DIRS})

add_library(ColorMapper color_mapper.cu)