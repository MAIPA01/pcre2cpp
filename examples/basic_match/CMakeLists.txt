set(EXAMPLE_NAME pcre2cpp_basic_match)

add_executable(${EXAMPLE_NAME} main.cpp)

target_link_libraries(${EXAMPLE_NAME} pcre2cpp::pcre2cpp)