if(NOT PCRE2CPP_PCRE2_EXTERNAL OR (NOT TARGET pcre2-8 AND NOT TARGET pcre2-16 AND NOT TARGET pcre2-16))
    set(DOWNLOAD_PCRE2 ON)
else()
    set(DOWNLOAD_PCRE2 OFF)
endif()

if(NOT PCRE2CPP_MSTD_EXTERNAL OR NOT TARGET mstd::mstd)
    set(DOWNLOAD_MSTD ON)
else()
    set(DOWNLOAD_MSTD OFF)
endif()

if((PCRE2CPP_BUILD_TESTS OR PCRE2CPP_BUILD_COVERAGE) AND NOT TARGET gtest_main)
    set(DOWNLOAD_GTEST ON)
else()
    set(DOWNLOAD_GTEST OFF)
endif()

if(DOWNLOAD_PCRE2 OR DOWNLOAD_MSTD OR DOWNLOAD_GTEST)
    include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)
endif()

# PCRE2
if(DOWNLOAD_PCRE2)
    set(CMAKE_C_COMPILE_OPTIONS_VISIBILITY "-fvisibility=") # Trick for PCRE2

    if (PCRE2CPP_DISABLE_UTF8)
        set(PCRE2_BUILD_PCRE2_8 OFF)
    else()
        set(PCRE2_BUILD_PCRE2_8 ON)
    endif()

    if (PCRE2CPP_DISABLE_UTF16)
        set(PCRE2_BUILD_PCRE2_16 OFF)
    else()
        set(PCRE2_BUILD_PCRE2_16 ON)
    endif()

    if (PCRE2CPP_DISABLE_UTF32)
        set(PCRE2_BUILD_PCRE2_32 OFF)
    else()
        set(PCRE2_BUILD_PCRE2_32 ON)
    endif()

    set(PCRE2_NEWLINE "ANY")

    CPMAddPackage(
            URI "gh:PCRE2Project/pcre2#pcre2-10.47"
            OPTIONS "PCRE2_BUILD_TESTS OFF"
                    "PCRE2_BUILD_PCRE2GREP OFF"
                    "PCRE2_SUPPORT_UNICODE ON"
                    "CMAKE_C_VISIBILITY_PRESET hidden"
                    "CMAKE_VISIBILITY_INLINES_HIDDEN ON"
    )
endif()

# MSTD
if (DOWNLOAD_MSTD)
    CPMAddPackage("gh:maipa01/mstd#v1.5.2")
endif()

# GTEST
if(DOWNLOAD_GTEST)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()