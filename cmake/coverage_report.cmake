if(DEFINED _COVERAGE_REPORT_HELPER_INCLUDED)
    return()
endif()
set(_COVERAGE_REPORT_HELPER_INCLUDED TRUE)

function(setup_coverage_report)
    # CHECK COMPILER VERSION


    # CHECK IF PROGRAMS EXISTS
    find_program(LLVM_PROFDATA llvm-profdata REQUIRED)
    find_program(LLVM_COV llvm-cov REQUIRED)

    set(options CLEAN_TEMP)
    set(oneValueArgs TEST_TARGET FORMAT OUTPUT_DIR)
    set(multiValueArgs EXE_TARGETS)

    cmake_parse_arguments(PARSE_ARGV 0 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    # CHECK IF TEST TARGET WAS PROVIEDED
    if(NOT DEFINED COVERAGE_TEST_TARGET)
        message(FATAL_ERROR "Please provide explicitly TEST_TARGET.")
    # CHECK IF TEST TARGET IS TARGET
    elseif(NOT TARGET ${COVERAGE_TEST_TARGET})
        message(FATAL_ERROR "TEST_TARGET must be a TARGET.")
    endif()

    # CHECK IF TEST TARGET IS EXECUTABLE
    get_target_property(TEST_TARGET_TYPE ${COVERAGE_TEST_TARGET} TYPE)
    if(NOT TEST_TARGET_TYPE STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "TEST_TARGET must be an EXECUTABLE Target")
    endif()

    # DEFAULT FORMAT
    if(NOT DEFINED COVERAGE_FORMAT)
        set(COVERAGE_FORMAT html)
    endif()

    # DEFAULT OUTPUT_DIR
    if(NOT DEFINED COVERAGE_OUTPUT_DIR)
        set(COVERAGE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/coverage_report)
    endif()

    if(NOT EXISTS ${COVERAGE_OUTPUT_DIR})
        file(MAKE_DIRECTORY ${COVERAGE_OUTPUT_DIR})
    endif()

    # PROFDATA FILE PATH VARIABLE
    set(PROFDATA_FILE "${COVERAGE_OUTPUT_DIR}/coverage.profdata")
    file(TO_NATIVE_PATH ${PROFDATA_FILE} PROFDATA_FILE)

    # SETUP TARGETS LIST
    set(EXE_TARGETS "${COVERAGE_EXE_TARGETS};${COVERAGE_UNPARSED_ARGUMENTS}")
    foreach(EXE_TARGET ${EXE_TARGETS})
        if(NOT TARGET ${EXE_TARGET})
            message(FATAL_ERROR "EXE_TARGET must be a TARGET.")
        endif()

        get_target_property(TARGET_TYPE ${EXE_TARGET} TYPE)
        if(NOT TARGET_TYPE STREQUAL "EXECUTABLE")
            message(FATAL_ERROR "EXE_TARGET must be an EXECUTABLE Target")
        endif()
    endforeach()

    set(EXE_TARGETS "${COVERAGE_TEST_TARGET};${EXE_TARGETS}")

    set(COVERAGE_TARGET_NAME coverage_report_${COVERAGE_TEST_TARGET})

    # CREATE TARGET
    add_custom_target(${COVERAGE_TARGET_NAME}
        COMMAND rm -rf "${COVERAGE_OUTPUT_DIR}/*"
        COMMENT "Generating coverage report (format: ${COVERAGE_FORMAT}) in ${COVERAGE_OUTPUT_DIR}"
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        DEPENDS ${EXE_TARGETS}
        VERBATIM
    )

    # LIST OF PROFRAWS FOR EACH TARGET
    set(TARGETS_PROFRAWS "")
    set(TARGETS_EXECS "")
    foreach(TARGET_NAME ${EXE_TARGETS})
        # GET BINARY_DIR OF TARGET
        get_target_property(TARGET_BINARY_DIR ${TARGET_NAME} BINARY_DIR)

        # GET EXE FILE
        set(EXE_FILE "$<TARGET_FILE:${TARGET_NAME}>")

        list(APPEND TARGETS_EXECS ${EXE_FILE})

        # SET TARGET_PROFRAW PATH
        set(TARGET_PROFRAW "${TARGET_BINARY_DIR}/${TARGET_NAME}.profraw")
        file(TO_NATIVE_PATH ${TARGET_PROFRAW} TARGET_PROFRAW)

        # APPEND TO LIST
        list(APPEND TARGETS_PROFRAWS ${TARGET_PROFRAW})

        # ADD COMMAND
        add_custom_command(TARGET ${COVERAGE_TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${TARGET_PROFRAW} ${EXE_FILE}
            COMMENT "Running ${TARGET_NAME} to generate ${PROFRAW_TESTS}"
            WORKING_DIRECTORY ${TARGET_BINARY_DIR}
            VERBATIM
        )
    endforeach()

    # ADD MERGE COMMAND
    list(JOIN TARGETS_PROFRAWS " " PROFRAWS_TO_MERGE)
    add_custom_command(TARGET ${COVERAGE_TARGET_NAME} POST_BUILD
        COMMAND ${LLVM_PROFDATA} merge -sparse -instr -output=${PROFDATA_FILE} ${PROFRAWS_TO_MERGE}
        COMMENT "Merging coverage data into ${PROFDATA_FILE}"
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        VERBATIM
    )

    # ADD GENERATE REPORT COMMAND
    get_target_property(TEST_TARGET_SOURCE_DIR ${COVERAGE_TEST_TARGET} SOURCE_DIR)
    file(RELATIVE_PATH TEST_TARGET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_TARGET_SOURCE_DIR})
    file(TO_NATIVE_PATH ${TEST_TARGET_SOURCE_DIR} TEST_TARGET_SOURCE_DIR)
    set(TEST_FILES_REGEX ".*[\\\\\\\/]${TEST_TARGET_SOURCE_DIR}[\\\\\\\/].*")

    add_custom_command(TARGET ${COVERAGE_TARGET_NAME} POST_BUILD
        COMMAND ${LLVM_COV} show ${TARGETS_EXECS}
        -instr-profile=${PROFDATA_FILE} 
        -format=${COVERAGE_FORMAT} 
        -output-dir=${COVERAGE_OUTPUT_DIR} 
        -ignore-filename-regex=${TEST_FILES_REGEX} 
        -show-mcdc -show-line-counts -show-expansions 
        -show-instantiations -show-regions -show-line-counts-or-regions 
        -show-directory-coverage -use-color --show-branches percent
        COMMENT "Generating '${COVERAGE_FORMAT}' report..."
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        VERBATIM
    )

    # CLEAN TEMP
    if(${COVERAGE_CLEAN_TEMP})
        foreach(PROFRAW ${TARGETS_PROFRAWS})
            get_filename_component(PROFRAWS_DIR ${PROFRAW} DIRECTORY)

            add_custom_command(TARGET ${COVERAGE_TARGET_NAME} POST_BUILD
                COMMAND rm ${PROFRAW}
                COMMENT "Cleaning profraw file (${PROFRAW})..."
                WORKING_DIRECTORY ${PROFRAWS_DIR}
                VERBATIM
            )
        endforeach()

        add_custom_command(TARGET ${COVERAGE_TARGET_NAME} POST_BUILD
            COMMAND rm ${PROFDATA_FILE}
            COMMENT "Cleaning profdata file (${PROFDATA_FILE})..."
            WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
            VERBATIM
        )
    endif()
endfunction()