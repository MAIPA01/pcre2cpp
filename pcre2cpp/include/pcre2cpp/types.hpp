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
		#if _PCRE2CPP_HAS_EXCEPTIONS
	template<size_t utf>
	class basic_pcre2cpp_exception;
	template<size_t utf>
	class basic_regex_exception;
	template<size_t utf>
	class basic_match_result_exception;
		#endif

	template<size_t utf>
	class basic_match_result;
	template<size_t utf>
	class basic_regex;

		#if _PCRE2CPP_HAS_EXCEPTIONS
	using u8pcre2cpp_exception		= basic_pcre2cpp_exception<8>;
	using u16pcre2cpp_exception		= basic_pcre2cpp_exception<16>;
	using u32pcre2cpp_exception		= basic_pcre2cpp_exception<32>;
	using pcre2cpp_exception		= u8pcre2cpp_exception;

	using u8regex_exception			= basic_regex_exception<8>;
	using u16regex_exception		= basic_regex_exception<16>;
	using u32regex_exception		= basic_regex_exception<32>;
	using regex_exception			= u8regex_exception;

	using u8match_result_exception	= basic_match_result_exception<8>;
	using u16match_result_exception = basic_match_result_exception<16>;
	using u32match_result_exception = basic_match_result_exception<32>;
	using match_result_exception	= u8match_result_exception;
		#endif

	using u8match_result  = basic_match_result<8>;
	using u16match_result = basic_match_result<16>;
	using u32match_result = basic_match_result<32>;
	using match_result	  = u8match_result;

	using u8regex		  = basic_regex<8>;
	using u16regex		  = basic_regex<16>;
	using u32regex		  = basic_regex<32>;
	using regex			  = u8regex;
} // namespace pcre2cpp
	#endif
#endif