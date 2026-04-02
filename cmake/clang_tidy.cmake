if(DEFINED _CLANG_TIDY_HELPER_INCLUDED)
    return()
endif()
set(_CLANG_TIDY_HELPER_INCLUDED TRUE)

set(CLANG_TIDY_RUNNER ${CMAKE_CURRENT_LIST_DIR}/run_clang_tidy.cmake)

function(enable_clang_tidy)
    find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)

    set(options "")
    set(oneValueArgs TARGET CONFIG HEADER_FILTER EXCLUDE_HEADER_FILTER CXX_STANDARD)
    set(multiValueArgs "")

    cmake_parse_arguments(PARSE_ARGV 0 CLANG_TIDY "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if(NOT DEFINED CLANG_TIDY_TARGET)
        message(FATAL_ERROR "Please provide explicitly TARGET.")
    endif()

    if(NOT TARGET ${CLANG_TIDY_TARGET})
        message(FATAL_ERROR "Target '${CLANG_TIDY_TARGET}' does not exist.")
    endif()

    if(NOT DEFINED CLANG_TIDY_CONFIG)
        set(CLANG_TIDY_CONFIG ${CMAKE_SOURCE_DIR}/.clang-tidy)
        message(NOTICE "No .clang-tidy config file provided using default path ${CLANG_TIDY_CONFIG}")
    endif()

    get_target_property(TARGET_SOURCES ${CLANG_TIDY_TARGET} SOURCES)
    if(NOT TARGET_SOURCES)
        message(NOTICE "Target ${CLANG_TIDY_TARGET} has no sources - skipping clang-tidy...")
        return()
    endif()

    set(TIDY_SOURCES)
    foreach(SRC ${TARGET_SOURCES})
        get_source_file_property(TIDY_DISABLED ${SRC} CXX_CLANG_TIDY)

        if(NOT DEFINED TIDY_DISABLED OR NOT TIDY_DISABLED STREQUAL "")
            list(APPEND TIDY_SOURCES "\\\"${SRC}\\\"")
        endif()
    endforeach()
    string(JOIN ";" TIDY_SOURCES_STR ${TIDY_SOURCES})

    get_target_property(TARGET_SOURCE_DIR ${CLANG_TIDY_TARGET} SOURCE_DIR)
    file(RELATIVE_PATH TARGET_SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_SOURCE_DIR})

    set(CLANG_TIDY_ARGS
        "--quiet"
        "--system-headers=false"
        "--extra-arg=-Wno-macro-redefined"
    )

    if(DEFINED CLANG_TIDY_CXX_STANDARD)
        set(CLANG_TIDY_ARGS ${CLANG_TIDY_ARGS} "--extra-arg=-std=${CLANG_TIDY_CXX_STANDARD}")
    endif()

    if(DEFINED CLANG_TIDY_HEADER_FILTER)
        set(CLANG_TIDY_ARGS ${CLANG_TIDY_ARGS} "--header-filter=${CLANG_TIDY_HEADER_FILTER}")
    endif()

    if(DEFINED CLANG_TIDY_EXCLUDE_HEADER_FILTER)
        set(CLANG_TIDY_ARGS ${CLANG_TIDY_ARGS} "--exclude-header-filter=${CLANG_TIDY_EXCLUDE_HEADER_FILTER}")
    endif()

    string(JOIN ";" CLANG_TIDY_ARGS_STR ${CLANG_TIDY_ARGS})

    get_target_property(BINARY_DIR ${CLANG_TIDY_TARGET} BINARY_DIR)
    set(CACHE_DIR ${BINARY_DIR}/clang-tidy-cache)
    file(MAKE_DIRECTORY ${CACHE_DIR})

    set(CACHE_FILE ${CACHE_DIR}/${CLANG_TIDY_TARGET}.tidy)

    set_target_properties(${CLANG_TIDY_TARGET} PROPERTIES CXX_CLANG_TIDY
                          "${CLANG_TIDY_EXE};--config-file=${CLANG_TIDY_CONFIG};-p=${CMAKE_BINARY_DIR};${CLANG_TIDY_ARGS_STR}"
    )

    set(TIDY_TARGET clang_tidy_${CLANG_TIDY_TARGET})

    add_custom_target(
        ${TIDY_TARGET}
        COMMAND ${CMAKE_COMMAND}
            -DFORCE_TIDY=OFF
            -DCLANG_TIDY_EXE="${CLANG_TIDY_EXE}"
            -DCLANG_TIDY_ARGS="${CLANG_TIDY_ARGS}"
            -DCLANG_COMPILE_COMMANDS_DIR="${CMAKE_BINARY_DIR}"
            -DCLANG_TIDY_CONFIG="${CLANG_TIDY_CONFIG}"
            -DSOURCES="${TIDY_SOURCES_STR}"
            -DCACHE_FILE="${CACHE_FILE}"
            -P "${CLANG_TIDY_RUNNER}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Running clang-tidy for target ${CLANG_TIDY_TARGET}"
    )

    add_custom_target(
        ${TIDY_TARGET}_full
        COMMAND ${CMAKE_COMMAND}
            -DFORCE_TIDY=ON
            -DCLANG_TIDY_EXE="${CLANG_TIDY_EXE}"
            -DCLANG_TIDY_ARGS="${CLANG_TIDY_ARGS}"
            -DCLANG_COMPILE_COMMANDS_DIR="${CMAKE_BINARY_DIR}"
            -DCLANG_TIDY_CONFIG="${CLANG_TIDY_CONFIG}"
            -DSOURCES="${TIDY_SOURCES_STR}"
            -DCACHE_FILE="${CACHE_FILE}"
            -P "${CLANG_TIDY_RUNNER}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Running full clang-tidy for target ${CLANG_TIDY_TARGET}"
    )

    message(STATUS "clang-tidy target added: ${TIDY_TARGET}")
endfunction()

function(disable_clang_tidy_for_file FILE)

    set_source_files_properties(
        "${FILE}"
        PROPERTIES
        CXX_CLANG_TIDY ""
    )

    message(STATUS "clang-tidy disabled for: ${FILE}")
endfunction()