target_sources(opensaft
PUBLIC
	ReconSettings.h
	Transducer.h
	Saft.h
PRIVATE
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
		Slicer/ColorMapper.cuh
	PRIVATE
		Saft.cu
		Slicer/ColorMapper.cu
	)
endif()

add_subdirectory(Util)
add_subdirectory(Memory)

if (OPENSAFT_GUI)
	add_subdirectory(Gui)
	add_subdirectory(Slicer)
endif ()