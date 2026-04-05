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
	/**
	 * @brief Match options
	 * @ingroup pcre2cpp
	 */
	enum class match_options_bits : uint32_t {
		/// @brief No options set (default)
		None					= 0u,
		/// @brief Match only at the first position
		Anchored				= PCRE2_ANCHORED,
		/// @brief On success, make a private subject copy
		CopyMatchedSubject		= PCRE2_COPY_MATCHED_SUBJECT,
		/// @brief Only useful in rare cases; use with care
		DisableRecurseLoopCheck = PCRE2_DISABLE_RECURSELOOP_CHECK,
		/// @brief Pattern can match only at end of subject
		EndAnchored				= PCRE2_ENDANCHORED,
		/// @brief Subject string is not the beginning of a line
		NotBOL					= PCRE2_NOTBOL,
		/// @brief Subject string is not the end of a line
		NotEOL					= PCRE2_NOTEOL,
		/// @brief An empty string is not a valid match
		NotEmpty				= PCRE2_NOTEMPTY,
		/// @brief An empty string at the start of the subject is not a valid match
		NotEmptyAtStart			= PCRE2_NOTEMPTY_ATSTART,
		/// @brief Do not use JIT matching
		NoJIT					= PCRE2_NO_JIT,
		/// @brief Do not check the subject for UTF validity(only relevant if compile_options::UTF was set at compile time)
		NoUTFCheck				= PCRE2_NO_UTF_CHECK,
		/// @brief Return match_error_codes::Partial for a partial match even if there is a full match
		PartialHard				= PCRE2_PARTIAL_HARD,
		/// @brief Return match_error_codes::Partial for a partial match if no full matches are found
		PartialSoft				= PCRE2_PARTIAL_SOFT
	};

	/**
	 * @brief Match options flags group
	 * @ingroup pcre2cpp
	 */
	using match_options = mstd::flags<match_options_bits>;

	/**
	 * @brief operator for combining match options to one flags group
	 * @ingroup pcre2cpp
	 * @param opt0 first match option
	 * @param opt1 second match option
	 * @return Match options flags group created from two match options
	 */
	static _PCRE2CPP_CONSTEXPR17 match_options operator|(const match_options_bits opt0, const match_options_bits opt1) noexcept {
		return mstd::operator|(opt0, opt1);
	}
} // namespace pcre2cpp
	#endif
#endif