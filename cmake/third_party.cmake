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
message(STATUS "Include CPM.cmake from ${CPM_DOWNLOAD_LOCATION}")
include(${CPM_DOWNLOAD_LOCATION})

# Add pcre2 library
if (NOT ${PCRE2CPP_USE_EXTERNAL_PCRE2})
    CPMAddPackage(
            URI "gh:PCRE2Project/pcre2#pcre2-10.47"
            OPTIONS "PCRE2_BUILD_PCRE2_8 ON"
                    "PCRE2_BUILD_PCRE2_16 ON"
                    "PCRE2_BUILD_PCRE2_32 ON"
                    "PCRE2_BUILD_TESTS OFF"
                    "PCRE2_BUILD_PCRE2GREP OFF"
                    "PCRE2_SUPPORT_UNICODE ON"
    )
else()
    set(PCRE2_BUILD_PCRE2_8 ON)
    set(PCRE2_BUILD_PCRE2_16 ON)
    set(PCRE2_BUILD_PCRE2_32 ON)
    set(PCRE2_BUILD_TESTS OFF)
    set(PCRE2_BUILD_PCRE2GREP OFF)
    set(PCRE2_SUPPORT_UNICODE ON)
endif()

set(PCRE2_NEWLINE "ANY")

if (NOT ${PCRE2CPP_USE_EXTERNAL_MSTD})
    CPMAddPackage(
            URI "gh:maipa01/mstd#v1.5.1"
            OPTIONS "MSTD_ENABLE_CXX20 ${PCRE2CPP_ENABLE_CXX20}"
                    "MSTD_DISABLE_ASSERT_ON_RELEASE ${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE}"
    )
else()
    set(MSTD_ENABLE_CXX20 ${PCRE2CPP_ENABLE_CXX20})
    set(MSTD_DISABLE_ASSERT_ON_RELEASE ${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE})
endif()

if(PCRE2CPP_BUILD_TESTS OR PCRE2CPP_BUILD_COVERAGE)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()