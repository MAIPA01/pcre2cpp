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
#ifndef _PCRE2CPP_REGEX_MATCH_OPTIONS_HPP_
	#define _PCRE2CPP_REGEX_MATCH_OPTIONS_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/types.hpp>

namespace pcre2cpp {
	enum class match_options_bits : uint32_t {
		None					= 0u,							   // No options set (default)
		Anchored				= PCRE2_ANCHORED,				   // Match only at the first position
		CopyMatchedSubject		= PCRE2_COPY_MATCHED_SUBJECT,	   // On success, make a private subject copy
		DisableRecurseLoopCheck = PCRE2_DISABLE_RECURSELOOP_CHECK, // Only useful in rare cases; use with care
		EndAnchored				= PCRE2_ENDANCHORED,			   // Pattern can match only at end of subject
		NotBOL					= PCRE2_NOTBOL,					   // Subject string is not the beginning of a line
		NotEOL					= PCRE2_NOTEOL,					   // Subject string is not the end of a line
		NotEmpty				= PCRE2_NOTEMPTY,				   // An empty string is not a valid match
		NotEmptyAtStart			= PCRE2_NOTEMPTY_ATSTART, // An empty string at the start of the subject is not a valid match
		NoJIT					= PCRE2_NO_JIT,			  // Do not use JIT matching
		NoUTFCheck =
		  PCRE2_NO_UTF_CHECK, // Do not check the subject for UTF validity(only relevant if compile_options::UTF was set at compile time)
		PartialHard = PCRE2_PARTIAL_HARD, // Return match_error_codes::Partial for a partial match even if there is a full match
		PartialSoft = PCRE2_PARTIAL_SOFT  // Return match_error_codes::Partial for a partial match if no full matches are found
	};

	using match_options = mstd::flags<match_options_bits>;

	static _PCRE2CPP_CONSTEXPR17 match_options operator|(const match_options_bits opt0,
	  const match_options_bits opt1) noexcept {
		return mstd::operator|(opt0, opt1);
	}
} // namespace pcre2cpp
	#endif
#endif