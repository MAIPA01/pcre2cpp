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
#include "pcre2cpp_config.hpp"
#include "pcre2cpp_libs.hpp"

namespace pcre2cpp {
	template<size_t utf> class basic_pcre2cpp_exception;
	template<size_t utf> class basic_regex_exception;
	template<size_t utf> class basic_match_result_exception;
	template<size_t utf> class basic_match_result;
	template<size_t utf> class basic_regex;

	using pcre2cpp_exception = basic_pcre2cpp_exception<8>;
	using wpcre2cpp_exception = basic_pcre2cpp_exception<16>;
	using pcre2cpp_exception_32 = basic_pcre2cpp_exception<32>;

	using regex_exception = basic_regex_exception<8>;
	using wregex_exception = basic_regex_exception<16>;
	using regex_exception_32 = basic_regex_exception<32>;

	using match_result_exception = basic_match_result_exception<8>;
	using wmatch_result_exception = basic_match_result_exception<16>;
	using match_result_exception_32 = basic_match_result_exception<32>;

	using match_result = basic_match_result<8>;
	using wmatch_result = basic_match_result<16>;
	using match_result_32 = basic_match_result<32>;

	using regex = basic_regex<8>;
	using wregex = basic_regex<16>;
	using regex_32 = basic_regex<32>;
}