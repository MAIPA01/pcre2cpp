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
#ifndef _PCRE2CPP_PCRE2CPP_HPP_
	#define _PCRE2CPP_PCRE2CPP_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/exceptions/exceptions.hpp>
		#include <pcre2cpp/libs.hpp>
		#include <pcre2cpp/match/match_result.hpp>
		#include <pcre2cpp/regex/regex.hpp>
		#include <pcre2cpp/regex/regex_compile_options.hpp>
		#include <pcre2cpp/regex/regex_match_options.hpp>
		#include <pcre2cpp/types.hpp>
		#include <pcre2cpp/utils/pcre2_data.hpp>
		#if _PCRE2CPP_HAS_ASSERTS
			#include <pcre2cpp/utils/assert.hpp>
		#endif

	#endif
#endif