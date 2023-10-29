include(FetchContent)

find_package(ImGUI REQUIRED)

FetchContent_Declare(
  ImGuiFileDialog
  GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog
  GIT_TAG 72ff16ef1806da070f2d28f9e34b130d49f01143
)
FetchContent_GetProperties(ImGuiFileDialog)
if (NOT ImGuiFileDialog_POPULATED)
  FetchContent_Populate(ImGuiFileDialog)
  add_library(ImGuiFileDialog_target
    ${imguifiledialog_SOURCE_DIR}/ImGuiFileDialog/ImGuiFileDialog.cpp
    )

  target_link_libraries(ImGuiFileDialog_target PUBLIC 
    ${ImGUI_LIBRARIES})
	
  target_include_directories(ImGuiFileDialog_target PUBLIC
    ${imgui_SOURCE_DIR}
    ${imguifiledialog_SOURCE_DIR}
  )
endif()

SET(ImGUIFileDialog_INCLUDE_DIR "${imguifiledialog_SOURCE_DIR}/ImGuiFileDialog")
SET(ImGUIFileDialog_LIBRARIES ImGuiFileDialog_target)