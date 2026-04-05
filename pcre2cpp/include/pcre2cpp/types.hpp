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
#ifndef _PCRE2CPP_TYPES_HPP_
	#define _PCRE2CPP_TYPES_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/libs.hpp>

namespace pcre2cpp {
	enum class utf_type : uint8_t;

	namespace utils {
		template<utf_type utf>
		struct pcre2_data;
	}

		#if _PCRE2CPP_HAS_EXCEPTIONS
	template<utf_type utf>
	class basic_pcre2cpp_exception;
	template<utf_type utf>
	class basic_regex_exception;
	template<utf_type utf>
	class basic_match_result_exception;
		#endif

	template<utf_type utf>
	struct basic_match_value;
	struct sub_match_value;
	template<utf_type utf>
	class basic_match_result;
	template<utf_type utf>
	class basic_regex;
} // namespace pcre2cpp
	#endif
#endif