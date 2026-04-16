cmake_minimum_required(VERSION 3.30)
project(pcre2cpp VERSION 1.2.5 LANGUAGES CXX)

option(BUILD_SHARED_LIBS 					"Build shared library" 			OFF)
option(PCRE2CPP_BUILD_TESTS 				"Build tests"			 		${PROJECT_IS_TOP_LEVEL})
option(PCRE2CPP_BUILD_BENCHMARK  			"Build benchmark" 				${PROJECT_IS_TOP_LEVEL})
option(PCRE2CPP_BUILD_COVERAGE 				"Enable coverage reporting" 	${PROJECT_IS_TOP_LEVEL})
option(PCRE2CPP_BUILD_DOCUMENTATION	 		"Build documentation"	 		${PROJECT_IS_TOP_LEVEL})

option(PCRE2CPP_ENABLE_CLANG_TIDY 			"Enables clang-tidy checks" 	${PROJECT_IS_TOP_LEVEL})

option(PCRE2CPP_INSTALL 					"Enables installation of this project"						${PROJECT_IS_TOP_LEVEL})
option(PCRE2CPP_INSTALL_TEST				"This is only to test if installation of pcre2cpp works" 	OFF)

option(PCRE2CPP_MSTD_EXTERNAL 			"Uses users own mstd library (tested and compatible with: 1.5.2)"		OFF)
option(PCRE2CPP_PCRE2_EXTERNAL 			"Uses users own pcre2 library (tested and compatible with: 10.47)"		OFF)

option(PCRE2CPP_ENABLE_CXX20 					"Enables C++20 features" 							OFF)
option(PCRE2CPP_DISABLE_ASSERT_ON_RELEASE 		"Disables assert on release builds" 				OFF)
option(PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS   	"Changes every assert to exceptions in pcre2cpp" 	OFF)
option(PCRE2CPP_DISABLE_UTF8  					"Disables UTF-8 support" 							OFF)
option(PCRE2CPP_DISABLE_UTF16					"Disables UTF-16 support" 							OFF)
option(PCRE2CPP_DISABLE_UTF32 					"Disables UTF-32 support" 							OFF)

message(STATUS "PCRE2CPP_ENABLE_CXX20: ${PCRE2CPP_ENABLE_CXX20}")
message(STATUS "PCRE2CPP_DISABLE_ASSERT_ON_RELEASE: ${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE}")
message(STATUS "PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS: ${PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS}")
message(STATUS "PCRE2CPP_DISABLE_UTF8: ${PCRE2CPP_DISABLE_UTF8}")
message(STATUS "PCRE2CPP_DISABLE_UTF16: ${PCRE2CPP_DISABLE_UTF16}")
message(STATUS "PCRE2CPP_DISABLE_UTF32: ${PCRE2CPP_DISABLE_UTF32}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

if (PCRE2CPP_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()

if (PCRE2CPP_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if (NOT PCRE2CPP_INSTALL_TEST)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pcre2cpp)
endif()

if (PCRE2CPP_BUILD_TESTS OR PCRE2CPP_BUILD_COVERAGE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (PCRE2CPP_BUILD_COVERAGE OR PCRE2CPP_BUILD_BENCHMARK)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/benchmark)
endif()

if (PCRE2CPP_BUILD_COVERAGE)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

	setup_coverage_report(
			TEST_TARGET pcre2cpp-tests
			FORMAT html
			OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
			CLEAN_TEMP
	)
endif()