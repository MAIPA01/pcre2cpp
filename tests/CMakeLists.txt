project(pcre2cpp-tests VERSION 1.2.4 LANGUAGES CXX)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

enable_testing()

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})
add_test(${PROJECT_NAME}_TEST ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE pcre2cpp gtest_main)

# Konfiguracja PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
        # MSVC/clang with MSVC frontend
        $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
            $<$<CONFIG:Release>:
                /O2
                /GL
                /fp:fast
            >
            $<$<CONFIG:Debug>:
                /RTC1
                /Zi
            >
            /Zc:preprocessor
            /Zc:__cplusplus
            /W4
            /WX
        >
        # GCC/clang with GNU frontend
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
            $<$<CONFIG:Release>:
                -O3
                -flto
            >
            -Wall
            -Wextra
            -Werror
            -fno-strict-aliasing
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:
            $<$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>:
                --coverage
                -fcoverage-mcdc
                -fprofile-instr-generate
                -fcoverage-mapping
            >
        >
)

target_link_options(${PROJECT_NAME} PRIVATE
        # MSVC/clang with MSVC frontend
        $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
            $<$<CONFIG:Release>:/LTCG>
            $<$<CONFIG:Debug>:/DEBUG>
        >
        # GCC/clang with GNU frontend
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
            $<$<CONFIG:Release>:-flto>
            -static
        >
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:
            $<$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>:
                --coverage
                -fprofile-instr-generate
            >
        >
)