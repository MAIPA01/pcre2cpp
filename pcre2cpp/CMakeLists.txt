# header files
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
file(GLOB_RECURSE ${PROJECT_HEADERS} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

# make library
add_library(${PROJECT_NAME} INTERFACE ${${PROJECT_HEADERS}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})

if (PCRE2CPP_ENABLE_CLANG_TIDY)
    if (PCRE2CPP_ENABLE_CXX20)
        set(CLANG_TIDY_CXX_STANDARD c++20)
    else()
        set(CLANG_TIDY_CXX_STANDARD c++17)
    endif()

    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/pch.hpp)
    enable_clang_tidy(
            TARGET ${PROJECT_NAME}
            CONFIG "${CMAKE_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_CXX_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${CMAKE_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )
endif()

target_compile_features(${PROJECT_NAME} INTERFACE $<IF:$<BOOL:${PCRE2CPP_ENABLE_CXX20}>, cxx_std_20, cxx_std_17>)

# set headers folder
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} INTERFACE mstd
                                                $<$<TARGET_EXISTS:pcre2-8>:pcre2-8>
                                                $<$<TARGET_EXISTS:pcre2-16>:pcre2-16>
                                                $<$<TARGET_EXISTS:pcre2-32>:pcre2-32>
)

target_precompile_headers(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/pch.hpp)

target_compile_definitions(${PROJECT_NAME} INTERFACE    $<$<BOOL:${PCRE2CPP_ENABLE_CXX20}>:PCRE2CPP_ENABLE_CXX20>
                                                        $<$<BOOL:${PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS}>:PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS>
                                                        $<$<BOOL:${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE}>:PCRE2CPP_DISABLE_ASSERT_ON_RELEASE>
                                                        $<$<BOOL:${PCRE2CPP_DISABLE_UTF8}>:PCRE2CPP_DISABLE_UTF8>
                                                        $<$<BOOL:${PCRE2CPP_DISABLE_UTF16}>:PCRE2CPP_DISABLE_UTF16>
                                                        $<$<BOOL:${PCRE2CPP_DISABLE_UTF32}>:PCRE2CPP_DISABLE_UTF32>
)

# Optimizations
target_compile_options(${PROJECT_NAME} INTERFACE
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

target_link_options(${PROJECT_NAME} INTERFACE
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