/*
 * pcre2cpp_types.hpp
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 * This file is part of the pcre2cpp project, which is licensed under the BSD 3-Clause License with Attribution Requirement.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions, and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software must display the following acknowledgment:
 *    "This product includes software developed by Patryk Antosik (MAIPA01) as part of the pcre2cpp project."
 * 4. If you modify this software and distribute it, you must include a notice stating that the original work was created by Patryk Antosik (MAIPA01) as part of the pcre2cpp project.
 * 5. Neither the name of the author nor the name "pcre2cpp" may be used to endorse or promote products derived from this software without specific prior written permission.
 * 6. Sale of unmodified code: The unmodified version of this software may not be sold without the explicit permission of the author.
 * 7. Sale of modified code: Modified versions of the code may be sold, provided that:
 *    - Proper attribution to Patryk Antosik (MAIPA01) as the original author is maintained.
 *    - The modified code is clearly marked as a modified version.
 *    - The modified code must be accompanied by documentation that includes references to the original version and its author.
 * 8. Exception for large projects: If this code is used as part of a larger project (e.g., an application, framework, or other libraries), permission is not required for the sale of the unmodified version, as long as:
 *    - The project is more than just a wrapper for the original code.
 *    - The code is an integral part of the larger project, and the project does not merely involve selling the original code.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * PCRE2 Exception:
 * Notwithstanding the above, you are permitted to link or otherwise combine this software with the PCRE2 library (or a modified version of that library), and distribute the resulting binaries without restriction, provided that you do not modify the PCRE2 library itself.
 * Furthermore, the second condition in the BSD license (covering binary redistributions) does not apply to software that includes this wrapper as part of a larger binary package, unless the package itself uses PCRE2 independently.
 */

 /*
 * This file is part of the pcre2cpp project, which includes code from the PCRE2 library.
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 * This project is licensed under the BSD 3-Clause License with Attribution Requirement.
 *
 * PCRE2 library included in this project:
 * Copyright (c) 2016-2024, University of Cambridge.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notices, this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notices, this list of conditions, and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Cambridge nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * Exemption for binary redistributions applies as per PCRE2's BSD license terms.
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