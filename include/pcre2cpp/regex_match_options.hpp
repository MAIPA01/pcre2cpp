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
#include "pcre2cpp_types.hpp"

namespace pcre2cpp {
	enum class regex_match_options_bits : uint32_t {
		NONE = 0u,														// No options set (default)
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

	using regex_match_options = uint32_t;

	constexpr static regex_match_options operator| (const regex_match_options_bits& opt0, const regex_match_options_bits& opt1) noexcept {
		return static_cast<regex_match_options>(static_cast<uint32_t>(opt0) | static_cast<uint32_t>(opt1));
	}

	constexpr static regex_match_options operator| (const regex_match_options& opts, const regex_match_options_bits& opt) noexcept {
		return static_cast<regex_match_options>(opts | static_cast<uint32_t>(opt));
	}
}