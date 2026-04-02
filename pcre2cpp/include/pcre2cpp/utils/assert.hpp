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
#ifndef _PCRE2CPP_ASSERT_HPP_
	#define _PCRE2CPP_ASSERT_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_ASSERTS
_PCRE2CPP_ERROR("This is only available for c++17 and greater and when asserts are enabled!");
	#else

		#include <mstd/mstd.hpp>

	/**
	 * @def pcre2cpp_assert(expression, ...)
	 * @brief pcre2cpp assert
	 * @ingroup utils
	 */

		#if _DEBUG
			#define pcre2cpp_assert(expression, ...)                                                                \
				MSTD_STOP_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#elif !defined(PCRE2CPP_DISABLE_ASSERT_ON_RELEASE)
			#define pcre2cpp_assert(expression, ...)                                                               \
				MSTD_LOG_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#else
			#define pcre2cpp_assert(expression, ...)                                                                 \
				MSTD_EMPTY_ASSERT_BASE(expression, [](const std::string_view) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#endif

	#endif
#endif