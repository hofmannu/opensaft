target_sources(opensaft
PUBLIC
	ColorMapper.h
	Interface.h
	ReconSettings.h
	Transducer.h
	Saft.h
PRIVATE
	ColorMapper.cpp
	Interface.cpp
	ReconSettings.cpp
	Transducer.cpp
	Saft.cpp
	)

if (OPENSAFT_CUDA_SUPPORT)
	set_property(TARGET opensaft PROPERTY CUDA_SEPARABLE_COMPILATION ON)
	set_property(TARGET opensaft PROPERTY CUDA_ARCHITECTURES 50 75 86)
	target_sources(opensaft
	PUBLIC
		Saft.cuh
		ColorMapper.cuh
	PRIVATE
		Saft.cu
		ColorMapper.cu
	)
endif()

target_link_libraries(opensaft PUBLIC
	${ImGUI_LIBRARIES}
	${implot_LIBRARIES}
	${GLFW_LIBRARIES}
	${cvolume_LIBRARIES}
	${ImGUIFileDialog_LIBRARIES}
	${backward_LIBRARIES}
)

target_include_directories(opensaft PUBLIC
	${ImGUI_INCLUDE_DIR}
	${ImPlot_INCLUDE_DIR}
	${ImGUIFileDialog_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
	${GLFW3_INCLUDE_DIR}
	${cvolume_INCLUDE_DIR}
	${backward_INCLUDE_DIR}
)
