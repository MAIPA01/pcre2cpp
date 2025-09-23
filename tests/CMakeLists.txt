project(pcre2cpp-tests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(GLSL_STRUCT_ENABLE_CXX20 ON CACHE INTERNAL "" FORCE)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})

target_link_libraries(${PROJECT_NAME} PRIVATE pcre2cpp gtest_main)

# Konfiguracja PCH
set(PCH_HEADER "${CMAKE_SOURCE_DIR}/pch.hpp")
if(EXISTS ${PCH_HEADER})
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})
endif()

if(MSVC)
	target_compile_options(${PROJECT_NAME} PUBLIC /Zc:preprocessor)
endif()