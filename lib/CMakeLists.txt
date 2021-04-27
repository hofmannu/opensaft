set(OpenGL_GL_PREFERENCE GLVND)
cmake_policy(SET CMP0079 NEW)

# clean imgui libraries
add_library(ImGui imgui/imgui.cpp)
target_link_libraries(ImGui PUBLIC ImGuiWidgets ImGuiTables) 

add_library(ImGuiWidgets imgui/imgui_widgets.cpp)
target_link_libraries(ImGuiWidgets PUBLIC ImGuiDraw)

add_library(ImGuiDraw imgui/imgui_draw.cpp)

add_library(ImGuiTables imgui/imgui_tables.cpp)


add_subdirectory(CVolume/)
