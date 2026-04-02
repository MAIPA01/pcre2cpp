cmake_minimum_required(VERSION 3.30)
project(pcre2cpp VERSION 1.2.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS 					"Build shared library" 			OFF)
option(PCRE2CPP_BUILD_TESTS 				"Build tests"			 		OFF)
option(PCRE2CPP_BUILD_COVERAGE 				"Enable coverage reporting" 	OFF)
option(PCRE2CPP_BUILD_DOCUMENTATION	 		"Build documentation"	 		OFF)
option(PCRE2CPP_ENABLE_CLANG_TIDY 			"Enables clang-tidy checks" 	OFF)

option(PCRE2CPP_USE_EXTERNAL_MSTD 			"Uses users own mstd library (tested and compatible with: 1.5.0)"		OFF)
option(PCRE2CPP_USE_EXTERNAL_PCRE2 			"Uses users own pcre2 library (tested and compatible with: 10.44)"		OFF)

option(PCRE2CPP_ENABLE_CXX20 					"Enables C++20 features" 							OFF)
option(PCRE2CPP_DISABLE_ASSERT_ON_RELEASE 		"Disables assert on release builds" 				OFF)
option(PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS   	"Changes every assert to exceptions in pcre2cpp" 	OFF)

message(STATUS "PCRE2CPP_ENABLE_CXX20: ${PCRE2CPP_ENABLE_CXX20}")
message(STATUS "PCRE2CPP_DISABLE_ASSERT_ON_RELEASE: ${PCRE2CPP_DISABLE_ASSERT_ON_RELEASE}")
message(STATUS "PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS: ${PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)
if (PCRE2CPP_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()
if (PCRE2CPP_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pcre2cpp)

if (PCRE2CPP_BUILD_TESTS OR PCRE2CPP_BUILD_COVERAGE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
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

# TODO: zrob install cos nie dziala z TARGETS pcre2-8

# library installation
#set(PROJECT_CONFIG_FILE ${PROJECT_NAME}-config)
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_CONFIG_FILE}
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#	RUNTIME DESTINATION bin
#	INCLUDES DESTINATION ${INSTALL_HEADERS_PATH}
#)

#install(DIRECTORY ${BUILD_HEADERS_PATH}/ DESTINATION ${INSTALL_HEADERS_PATH})
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

# export cmake config
#install(EXPORT ${PROJECT_CONFIG_FILE} DESTINATION cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE})
#export(EXPORT ${PROJECT_CONFIG_FILE} NAMESPACE ${PROJECT_NAMESPACE})

# configuration version file
#set(PROJECT_CONFIG_VERSION_FILE ${PROJECT_CONFIG_FILE}-version.cmake)
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION_FILE}"
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY AnyNewerVersion
#)

#install(FILES
#	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_CONFIG_VERSION_FILE}"
#	DESTINATION cmake/${PROJECT_NAME}
#)