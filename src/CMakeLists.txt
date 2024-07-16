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

add_subdirectory(Util)
add_subdirectory(Memory)
