# https://github.com/hofmannu/CVolume.git

include(FetchContent)

FetchContent_declare(
    CVolume
    GIT_REPOSITORY https://github.com/hofmannu/CVolume.git
    GIT_TAG v0.1
  )

FetchContent_GetProperties(CVolume)
if (NOT cvolume_POPULATED)
	FetchContent_Populate(CVolume)
	add_subdirectory(${cvolume_SOURCE_DIR} ${cvolume_BINARY_DIR})
endif()

set(cvolume_FOUND TRUE)
set(cvolume_INCLUDE_DIR "${cvolume_SOURCE_DIR}/src")
set(cvolume_LIBRARIES "Volume")