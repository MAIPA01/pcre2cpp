set(EXAMPLE_NAME pcre2cpp_match_at_specified_offset_with_result)

add_executable(${EXAMPLE_NAME} main.cpp)

target_link_libraries(${EXAMPLE_NAME} pcre2cpp::pcre2cpp)