cmake_minimum_required(VERSION 3.16)
project(pcre2cpp VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_PCRE2CPP_TESTS         "Build pcre2cpp tests"          OFF)
option(PCRE2CPP_ENABLE_EXCEPTIONS   "Enable Exceptions in pcre2cpp" ON )

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

# header files
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
set(BUILD_HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB ${PROJECT_HEADERS} ${BUILD_HEADERS_PATH}/${PROJECT_NAME}/*.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${PROJECT_HEADERS}})

# make library
add_library(${PROJECT_NAME} INTERFACE ${${PROJECT_HEADERS}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

# set headers folder
set(INSTALL_HEADERS_PATH include/${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${BUILD_HEADERS_PATH}
	#$<BUILD_INTERFACE:${BUILD_HEADERS_PATH}>
	#$<INSTALL_INTERFACE:${INSTALL_HEADERS_PATH}>
)

# check pcre2 library
if(TARGET pcre2-8 AND TARGET pcre2-16 AND TARGET pcre2-32)
    target_include_directories(${PROJECT_NAME} INTERFACE ${PCRE2_SOURCE_DIR}/src ${PCRE2_BINARY_DIR})
	target_link_libraries(${PROJECT_NAME} INTERFACE pcre2-8 pcre2-16 pcre2-32)
endif()

# Optional compiler flags
if(MSVC)
	target_compile_options(${PROJECT_NAME} INTERFACE /Zc:preprocessor)
else()
	#target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpendantic -Werror)
endif()

if(NOT PCRE2CPP_ENABLE_EXCEPTIONS)
    target_compile_definitions(${PROJECT_NAME} INTERFACE PCRE2CPP_NO_EXCEPTIONS)
endif()

# Optimizations
target_compile_options(${PROJECT_NAME} INTERFACE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Debug>:/RTC1>
        /Zc:preprocessor
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    >
)

target_link_options(${PROJECT_NAME} INTERFACE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        -static
        $<$<CONFIG:Release>:-fuse-ld=lld>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
    >
)

if(BUILD_PCRE2CPP_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

# TODO: zrób install coœ nie dzia³a z TARGETS pcre2-8

# library installation
#set(PROJECT_CONFIG_FILE ${PROJECT_NAME}-config)
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_CONFIG_FILE}
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#	RUNTIME DESTINATION bin
#	INCLUDES DESTINATION ${INSTALL_HEADERS_PATH}
#)

#install(DIRECTORY ${BUILD_HEADERS_PATH}/ DESTINATION ${INSTALL_HEADERS_PATH})
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

# export cmake config
#install(EXPORT ${PROJECT_CONFIG_FILE} DESTINATION cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE})
#export(EXPORT ${PROJECT_CONFIG_FILE} NAMESPACE ${PROJECT_NAMESPACE})

# configuration version file
#set(PROJECT_CONFIG_VERSION_FILE ${PROJECT_CONFIG_FILE}-version.cmake)
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION_FILE}"
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY AnyNewerVersion
#)

#install(FILES
#	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION_FILE}"
#	DESTINATION cmake/${PROJECT_NAME}
#)