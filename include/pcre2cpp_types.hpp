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
	template<size_t utf> class basic_regex_exception;
	template<size_t utf> class basic_match_result;
	template<size_t utf> class basic_regex;

	using regex_exception = basic_regex_exception<8>;
	using wregex_exception = basic_regex_exception<16>;
	using regex_exception_32 = basic_regex_exception<32>;

	using match_result = basic_match_result<8>;
	using wmatch_result = basic_match_result<16>;
	using match_result_32 = basic_match_result<32>;

	using regex = basic_regex<8>;
	using wregex = basic_regex<16>;
	using regex_32 = basic_regex<32>;

	enum class regex_compile_options : uint32_t {
		NONE = 0,											// No options set (default)
		ANCHORED = PCRE2_ANCHORED,							// Force pattern anchoring
		ALLOW_EMPTY_CLASS = PCRE2_ALLOW_EMPTY_CLASS,		// Allow empty classes
		ALT_BSUX = PCRE2_ALT_BSUX,							// Alternative handling of \u, \U, and \x
		ALT_CIRCUMFLEX = PCRE2_ALT_CIRCUMFLEX,				// Alternative handling of ^ in multiline mode
		ALT_VERBNAMES = PCRE2_ALT_VERBNAMES,				// Process backslashes in verb names
		AUTO_CALLOUT = PCRE2_AUTO_CALLOUT,					// Compile automatic callouts
		CASELESS = PCRE2_CASELESS,							// Do caseless matching
		DOLLAR_ENDONLY = PCRE2_DOLLAR_ENDONLY,				// $ not to match newline at end
		DOTALL = PCRE2_DOTALL,								// . matches anything including NL
		DUPNAMES = PCRE2_DUPNAMES,							// Allow duplicate names for subpatterns
		ENDANCHORED = PCRE2_ENDANCHORED,					// Pattern can match only at end of subject
		EXTENDED = PCRE2_EXTENDED,							// Ignore white space and # comments
		FIRSTLINE = PCRE2_FIRSTLINE,						// Force matching to be before newline
		LITERAL = PCRE2_LITERAL,							// Pattern characters are all literal
		MATCH_INVALID_UTF = PCRE2_MATCH_INVALID_UTF,		// Enable support for matching invalid UTF
		MATCH_UNSET_BACKREF = PCRE2_MATCH_UNSET_BACKREF,	// Match unset backreferences
		MULTILINE = PCRE2_MULTILINE,						// ^ and $ match newlines within data
		NEVER_BACKSLASH_C = PCRE2_NEVER_BACKSLASH_C,		// Lock out the use of \C in patterns
		NEVER_UCP = PCRE2_NEVER_UCP,						// Lock out PCRE2_UCP, e.g.via(*UCP)
		NEVER_UTF = PCRE2_NEVER_UTF,						// Lock out PCRE2_UTF, e.g.via(*UTF)
		NO_AUTO_CAPTURE = PCRE2_NO_AUTO_CAPTURE,			// Disable numbered capturing paren - theses(named ones available)
		NO_AUTO_POSSESS = PCRE2_NO_AUTO_POSSESS,			// Disable auto - possessification
		NO_DOTSTAR_ANCHOR = PCRE2_NO_DOTSTAR_ANCHOR,		// Disable automatic anchoring for .*
		NO_START_OPTIMIZE = PCRE2_NO_START_OPTIMIZE,		// Disable match - time start optimizations
		NO_UTF_CHECK = PCRE2_NO_UTF_CHECK,					// Do not check the pattern for UTF validity (only relevant if PCRE2_UTF is set)
		UCP = PCRE2_UCP,									// Use Unicode properties for \d, \w, etc.
		UNGREEDY = PCRE2_UNGREEDY,							// Invert greediness of quantifiers
		USE_OFFSET_LIMIT = PCRE2_USE_OFFSET_LIMIT,			// Enable offset limit for unanchored matching
		UTF = PCRE2_UTF,									// Treat pattern and subjects as UTF strings
	};

	enum class regex_match_options : uint32_t {
		NONE = 0,														// No options set (default)
		ANCHORED = PCRE2_ANCHORED,										// Match only at the first position
		COPY_MATCHED_SUBJECT = PCRE2_COPY_MATCHED_SUBJECT,				// On success, make a private subject copy
		DISABLE_RECURSELOOP_CHECK = PCRE2_DISABLE_RECURSELOOP_CHECK,	// Only useful in rare cases; use with care
		ENDANCHORED = PCRE2_ENDANCHORED,								// Pattern can match only at end of subject
		NOTBOL = PCRE2_NOTBOL,											// Subject string is not the beginning of a line
		NOTEOL = PCRE2_NOTEOL,											// Subject string is not the end of a line
		NOTEMPTY = PCRE2_NOTEMPTY,										// An empty string is not a valid match
		NOTEMPTY_ATSTART = PCRE2_NOTEMPTY_ATSTART,						// An empty string at the start of the subject is not a valid match
		NO_JIT = PCRE2_NO_JIT,											// Do not use JIT matching
		NO_UTF_CHECK = PCRE2_NO_UTF_CHECK,								// Do not check the subject for UTF validity(only relevant if PCRE2_UTF was set at compile time)
		PARTIAL_HARD = PCRE2_PARTIAL_HARD,								// Return PCRE2_ERROR_PARTIAL for a partial match even if there is a full match
		PARTIAL_SOFT = PCRE2_PARTIAL_SOFT								// Return PCRE2_ERROR_PARTIAL for a partial match if no full matches are found
	};
}