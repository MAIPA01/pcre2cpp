set(CPM_USE_LOCAL_PACKAGES OFF)
# Storage location
if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party)
endif()

# Set download location
if(NOT CPM_DOWNLOAD_LOCATION)
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/third_party/cpm/CPM.cmake")
endif()

# download CPM.cmake
# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)
if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
            https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
    )
endif()
message(STATUS "Include CPM.cmake from ${CPM_DOWNLOAD_LOCATION}")
include(${CPM_DOWNLOAD_LOCATION})