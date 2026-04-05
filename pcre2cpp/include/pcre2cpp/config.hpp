/*
 * pcre2cpp - PCRE2 cpp wrapper
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/pcre2cpp/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 *
 * PCRE2 library included in this project:
 * Copyright (c) 2016-2024, University of Cambridge.
 *
 * See the LICENSE_PCRE2 file for details: https://github.com/MAIPA01/pcre2cpp/blob/main/LICENSE_PCRE2
 */

#pragma once
#ifndef _PCRE2CPP_CONFIG_HPP_
	#define _PCRE2CPP_CONFIG_HPP_

/**
 *	@defgroup pcre2cpp PCRE2 C++
 *	@brief Main group
 */

/**
 * @defgroup utils Utilities
 * @brief group with utilities (not for end user to use)
 * @ingroup pcre2cpp
 */

/**
 * @namespace pcre2cpp
 * @brief Main namespace of pcre2cpp library
 */

/**
 * @namespace pcre2cpp::uitls
 * @brief Utility namespace of pcre2cpp library
 */

	#pragma region VERSION
	/**
	 * @brief pcre2cpp version major number
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION_MAJOR 1
	/**
	 * @brief pcre2cpp version minor number
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION_MINOR 2
	/**
	 * @brief pcre2cpp version patch number
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION_PATCH 3

	/**
	 * @brief stringify helper
	 * @ingroup utils
	 */
	#define _PCRE2CPP_STRINGIFY_HELPER(x) #x

	/**
	 * @brief converts version numbers to string
	 * @ingroup utils
	 */
	#define _PCRE2CPP_VERSION_TO_STRING(major, minor, patch)                                                          \
		_PCRE2CPP_STRINGIFY_HELPER(major) "." _PCRE2CPP_STRINGIFY_HELPER(minor) "." _PCRE2CPP_STRINGIFY_HELPER(patch)
	/**
	 * @brief converts version number to int
	 * @ingroup utils
	 */
	#define _PCRE2CPP_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

	/**
	 * @brief pcre2cpp version string
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION_STRING                                                                         \
		_PCRE2CPP_VERSION_TO_STRING(PCRE2CPP_VERSION_MAJOR, PCRE2CPP_VERSION_MINOR, PCRE2CPP_VERSION_PATCH)
	/**
	 * @brief pcre2cpp version int
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION_INT _PCRE2CPP_VERSION_TO_INT(PCRE2CPP_VERSION_MAJOR, PCRE2CPP_VERSION_MINOR, PCRE2CPP_VERSION_PATCH)
	/**
	 * @brief pcre2cpp version string
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_VERSION PCRE2CPP_VERSION_STRING
	#pragma endregion

	#pragma region LAST_UPDATE
	/**
	 * @brief pcre2cpp last update day
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_LAST_UPDATE_DAY 06
	/**
	 * @brief pcre2cpp last update month
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_LAST_UPDATE_MONTH 04
	/**
	 * @brief pcre2cpp last update year
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_LAST_UPDATE_YEAR 2026

	/**
	 * @brief converts last update date to string
	 * @ingroup utils
	 */
	#define _PCRE2CPP_LAST_UPDATE_DATE_HELPER(day, month, year)                                                    \
		_PCRE2CPP_STRINGIFY_HELPER(day) "." _PCRE2CPP_STRINGIFY_HELPER(month) "." _PCRE2CPP_STRINGIFY_HELPER(year)

	/**
	 * @brief pcre2cpp last update date string
	 * @ingroup pcre2cpp
	 */
	#define PCRE2CPP_LAST_UPDATE_DATE                                                                                      \
		_PCRE2CPP_LAST_UPDATE_DATE_HELPER(PCRE2CPP_LAST_UPDATE_DAY, PCRE2CPP_LAST_UPDATE_MONTH, PCRE2CPP_LAST_UPDATE_YEAR)
	#pragma endregion

	#pragma region CXX_VERSIONS
/**
 * @def _PCRE2CPP_HAS_CXX17
 * @brief check if compiler has c++ version greater or equal to c++17
 * @ingroup utils
 */
	#ifndef _HAS_CXX17
	// clang-format off
		#define _PCRE2CPP_HAS_CXX17 __cplusplus >= 201703l
	// clang-format on
	#else
		#define _PCRE2CPP_HAS_CXX17 _HAS_CXX17
	#endif

	/**
	 * @def _PCRE2CPP_HAS_CXX20
	 * @brief check if compiler has c++ version greater or equal to c++20 and if user enabled c++20 features using PCRE2CPP_ENABLE_CXX20
	 * @ingroup utils
	 */
	#ifndef PCRE2CPP_ENABLE_CXX20
		#define _PCRE2CPP_HAS_CXX20 0
	#elif !defined(_HAS_CXX20)
	// clang-format off
		#define _PCRE2CPP_HAS_CXX20 __cplusplus >= 202002l
	// clang-format on
	#else
		#define _PCRE2CPP_HAS_CXX20 _HAS_CXX20
	#endif
	#pragma endregion

/**
 * @def _PCRE2CPP_HAS_EXCEPTIONS
 * @brief check if exceptions are enabled
 * @ingroup utils
 */

/**
 * @def _PCRE2CPP_HAS_ASSERTS
 * @brief check if asserts are enabled
 * @ingroup utils
 */

	#ifdef PCRE2CPP_CHANGE_ASSERTS_TO_EXCEPTIONS
		#define _PCRE2CPP_HAS_EXCEPTIONS _PCRE2CPP_HAS_CXX17
		#define _PCRE2CPP_HAS_ASSERTS	 0
	#else
		#define _PCRE2CPP_HAS_EXCEPTIONS 0
		#define _PCRE2CPP_HAS_ASSERTS	 _PCRE2CPP_HAS_CXX17
	#endif

	#define PCRE2_CODE_UNIT_WIDTH 0

	#if _PCRE2CPP_HAS_EXCEPTIONS
		#define _PCRE2CPP_NOEXCEPT noexcept
	#else
		#define _PCRE2CPP_NOEXCEPT
	#endif

	#pragma region VERSION_SPECIFIC_VALUES
/**
 * @def _PCRE2CPP_CONSTEXPR17
 * @brief constexpr for c++17 and higher
 * @ingroup utils
 */
	#if _PCRE2CPP_HAS_CXX17
		#define _PCRE2CPP_CONSTEXPR17 constexpr
	#else
		#define _PCRE2CPP_CONSTEXPR17
	#endif

/**
 * @def _PCRE2CPP_CONSTEXPR20
 * @brief constexpr keyword for c++20 and higher
 * @ingroup utils
 */

/**
 * @def _PCRE2CPP_REQUIRES(condition)
 * @brief requires keyword for c++20 and higher
 * @ingroup utils
 */
	#if _PCRE2CPP_HAS_CXX20
		#define _PCRE2CPP_CONSTEXPR20		  constexpr
		#define _PCRE2CPP_REQUIRES(condition) requires (condition)
	#else
		#define _PCRE2CPP_CONSTEXPR20
		#define _PCRE2CPP_REQUIRES(condition)
	#endif
	#pragma endregion

	#include <mstd/mstd.hpp>

	/**
	 * @brief compiler message
	 * @ingroup utils
	 */
	#define _PCRE2CPP_MESSAGE(MESSAGE) _MSTD_MESSAGE(MESSAGE)
	/**
	 * @brief compiler warning
	 * @ingroup utils
	 */
	#define _PCRE2CPP_WARNING(MESSAGE) _MSTD_WARNING(MESSAGE)
	/**
	 * @brief compiler error
	 * @ingroup utils
	 */
	#define _PCRE2CPP_ERROR(MESSAGE) _MSTD_ERROR(MESSAGE)
#endif