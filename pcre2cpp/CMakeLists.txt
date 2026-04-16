# region HEADER FILES
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
set(BUILD_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INSTALL_HEADERS_DIR include)
file(GLOB_RECURSE ${PROJECT_HEADERS} CONFIGURE_DEPENDS ${BUILD_HEADERS_DIR}/*.hpp)
# endregion

# region LIBRARY_SETUP
add_library(${PROJECT_NAME} INTERFACE ${${PROJECT_HEADERS}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})
# endregion

# region CLANG_TIDY
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
# endregion

target_precompile_headers(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${BUILD_HEADERS_DIR}/${PROJECT_NAME}/pch.hpp>")

target_compile_features(${PROJECT_NAME} INTERFACE   $<BUILD_INTERFACE:$<IF:$<BOOL:${PCRE2CPP_ENABLE_CXX20}>, cxx_std_20, cxx_std_17>>)

# region LINK_LIBRARIES
if(NOT TARGET ${PROJECT_NAMESPACE}mstd_imported)
    add_library(${PROJECT_NAMESPACE}mstd_imported INTERFACE IMPORTED)
endif()

if(NOT TARGET ${PROJECT_NAMESPACE}pcre2-8_imported)
    add_library(${PROJECT_NAMESPACE}pcre2-8_imported INTERFACE IMPORTED)
endif()

if(NOT TARGET ${PROJECT_NAMESPACE}pcre2-16_imported)
    add_library(${PROJECT_NAMESPACE}pcre2-16_imported INTERFACE IMPORTED)
endif()

if(NOT TARGET ${PROJECT_NAMESPACE}pcre2-32_imported)
    add_library(${PROJECT_NAMESPACE}pcre2-32_imported INTERFACE IMPORTED)
endif()

target_link_libraries(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:
            mstd::mstd
            $<$<TARGET_EXISTS:pcre2-8>:pcre2-8>
            $<$<TARGET_EXISTS:pcre2-16>:pcre2-16>
            $<$<TARGET_EXISTS:pcre2-32>:pcre2-32>
        >
        $<INSTALL_INTERFACE:
            ${PROJECT_NAMESPACE}mstd_imported
            ${PROJECT_NAMESPACE}pcre2-8_imported
            ${PROJECT_NAMESPACE}pcre2-16_imported
            ${PROJECT_NAMESPACE}pcre2-32_imported
        >
)
# endregion

# set headers folder
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${BUILD_HEADERS_DIR}>)

# region COMPILE_DEFINITIONS
target_compile_definitions(${PROJECT_NAME} INTERFACE    $<BUILD_INTERFACE:
                                                            $<$<BOOL:${PCRE2CPP_ENABLE_CXX20}>: PCRE2CPP_ENABLE_CXX20
                                                                                                MSTD_ENABLE_CXX20
                                                            >
                                                            $<$<BOOL:${PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS}>:PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS>
                                                            $<$<BOOL:${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE}>:PCRE2CPP_DISABLE_ASSERT_ON_RELEASE
                                                                                                            MSTD_DISABLE_ASSERT_ON_RELEASE
                                                            >
                                                            $<$<BOOL:${PCRE2CPP_DISABLE_UTF8}>:PCRE2CPP_DISABLE_UTF8>
                                                            $<$<BOOL:${PCRE2CPP_DISABLE_UTF16}>:PCRE2CPP_DISABLE_UTF16>
                                                            $<$<BOOL:${PCRE2CPP_DISABLE_UTF32}>:PCRE2CPP_DISABLE_UTF32>
                                                        >
)
# endregion

# region COMPILE_OPTIONS
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
        $<BUILD_INTERFACE:$<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>>:
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >>
)
# endregion

# region LINK_OPTIONS
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
        $<BUILD_INTERFACE:$<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>>:
            --coverage
            -fprofile-instr-generate
        >>
)
# endregion

# region INSTALL
if(PCRE2CPP_INSTALL)
    # Installing header files
    install(DIRECTORY ${BUILD_HEADERS_DIR}/${PROJECT_NAME}/
            DESTINATION ${INSTALL_HEADERS_DIR}/${PROJECT_NAME}
            FILES_MATCHING PATTERN "*.hpp"
    )

    # Installing targets
    set(PROJECT_INSTALL_TARGETS ${PROJECT_NAME}Targets)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
            INCLUDES DESTINATION ${INSTALL_HEADERS_DIR}
    )

    # Generate Targets.cmake
    install(EXPORT ${PROJECT_INSTALL_TARGETS}
            FILE ${PROJECT_INSTALL_TARGETS}.cmake
            NAMESPACE ${PROJECT_NAMESPACE}
            DESTINATION cmake
    )

    # Create Config.cmake
    include(CMakePackageConfigHelpers)

    set(PROJECT_INSTALL_CONFIG ${PROJECT_NAME}Config)
    configure_package_config_file(
            "${CMAKE_SOURCE_DIR}/cmake/${PROJECT_INSTALL_CONFIG}.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            INSTALL_DESTINATION cmake
    )

    # Create Version.cmake
    set(PROJECT_INSTALL_VERSION ${PROJECT_NAME}Version)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Install config files
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            DESTINATION cmake
    )
endif()
# endregion