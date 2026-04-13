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
#ifndef _PCRE2CPP_COMPILE_OPTIONS_HPP_
	#define _PCRE2CPP_COMPILE_OPTIONS_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/types.hpp>

namespace pcre2cpp {
	/**
	 * @brief Compile options
	 * @ingroup pcre2cpp
	 */
	enum class compile_options_bits : uint32_t {
		/// @brief No options set (default)
		None			  = 0u,
		/// @brief Force pattern anchoring
		Anchored		  = PCRE2_ANCHORED,
		/// @brief Allow empty classes
		AllowEmptyClass	  = PCRE2_ALLOW_EMPTY_CLASS,
		/// @brief Alternative handling of \\u, \\U, and \\x
		AltBSUX			  = PCRE2_ALT_BSUX,
		/// @brief Alternative handling of ^ in multiline mode
		AltCircumflex	  = PCRE2_ALT_CIRCUMFLEX,
		/// @brief Process backslashes in verb names
		AltVerbNames	  = PCRE2_ALT_VERBNAMES,
		/// @brief Compile automatic callouts
		AutoCallout		  = PCRE2_AUTO_CALLOUT,
		/// @brief Do caseless matching
		Caseless		  = PCRE2_CASELESS,
		/// @brief $ not to match newline at end
		DollarEndonly	  = PCRE2_DOLLAR_ENDONLY,
		/// @brief . matches anything including NL
		DotAll			  = PCRE2_DOTALL,
		/// @brief Allow duplicate names for subpatterns
		DupNames		  = PCRE2_DUPNAMES,
		/// @brief Pattern can match only at end of subject
		EndAnchored		  = PCRE2_ENDANCHORED,
		/// @brief Ignore white space and # comments
		Extended		  = PCRE2_EXTENDED,
		/// @brief Force matching to be before newline
		FirstLine		  = PCRE2_FIRSTLINE,
		/// @brief Pattern characters are all literal
		Literal			  = PCRE2_LITERAL,
		/// @brief Enable support for matching invalid UTF
		MatchInvalidUTF	  = PCRE2_MATCH_INVALID_UTF,
		/// @brief Match unset backreferences
		MatchUnsetBackRef = PCRE2_MATCH_UNSET_BACKREF,
		/// @brief ^ and $ match newlines within data
		Multiline		  = PCRE2_MULTILINE,
		/// @brief Lock out the use of \\C in patterns
		NeverBackslashC	  = PCRE2_NEVER_BACKSLASH_C,
		/// @brief Lock out PCRE2_UCP, e.g.via(*UCP)
		NeverUCP		  = PCRE2_NEVER_UCP,
		/// @brief Lock out PCRE2_UTF, e.g.via(*UTF)
		NeverUTF		  = PCRE2_NEVER_UTF,
		/// @brief Disable numbered capturing paren - theses(named ones available)
		NoAutoCapture	  = PCRE2_NO_AUTO_CAPTURE,
		/// @brief Disable auto - possessification
		NoAutoPossess	  = PCRE2_NO_AUTO_POSSESS,
		/// @brief Disable automatic anchoring for .*
		NoDotStarAnchor	  = PCRE2_NO_DOTSTAR_ANCHOR,
		/// @brief Disable match - time start optimizations
		NoStartOptimize	  = PCRE2_NO_START_OPTIMIZE,
		/// @brief Do not check the pattern for UTF validity (only relevant if PCRE2_UTF is set)
		NoUTFCheck		  = PCRE2_NO_UTF_CHECK,
		/// @brief Use Unicode properties for \\d, \\w, etc.
		UCP				  = PCRE2_UCP,
		/// @brief Invert greediness of quantifiers
		UnGreedy		  = PCRE2_UNGREEDY,
		/// @brief Enable offset limit for unanchored matching
		UseOffsetLimit	  = PCRE2_USE_OFFSET_LIMIT,
		/// @brief Treat pattern and subjects as UTF strings
		UTF				  = PCRE2_UTF
	};

	/**
	 * @brief Compile options flags group
	 * @ingroup pcre2cpp
	 */
	using compile_options = mstd::flags<compile_options_bits>;

	/**
	 * @brief operator for combining compile options to one flags group
	 * @param opt0 first compile option
	 * @param opt1 second compile option
	 * @return Compile options flags group created from two compile options
	 */
	static _PCRE2CPP_CONSTEXPR17 compile_options operator|(const compile_options_bits opt0,
	  const compile_options_bits opt1) noexcept {
		return mstd::operator|(opt0, opt1);
	}
} // namespace pcre2cpp
	#endif
#endif