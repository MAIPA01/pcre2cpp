# for CPM
set(CPM_USE_LOCAL_PACKAGES OFF)

if(NOT CPM_SOURCE_CACHE)
    # Storage location
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party)
endif()

# Set download location
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

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
message(STATUS "include CPM.cmake from ${CPM_DOWNLOAD_LOCATION}")
include(${CPM_DOWNLOAD_LOCATION})

# Add pcre2 library
set(PCRE2_BUILD_PCRE2_8 ON)
set(PCRE2_BUILD_PCRE2_16 ON)
set(PCRE2_BUILD_PCRE2_32 ON)
CPMAddPackage("gh:PCRE2Project/pcre2#pcre2-10.44")

if(pcre2_ADDED)
    include_directories(${PCRE2_SOURCE_DIR}/src)

    message(STATUS "CMake added local pcre2: ${PCRE2_SOURCE_DIR}")
endif()