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
	enum class compile_options_bits : uint32_t {
		None			  = 0u,						   // No options set (default)
		Anchored		  = PCRE2_ANCHORED,			   // Force pattern anchoring
		AllowEmptyClass	  = PCRE2_ALLOW_EMPTY_CLASS,   // Allow empty classes
		AltBSUX			  = PCRE2_ALT_BSUX,			   // Alternative handling of \u, \U, and \x
		AltCircumflex	  = PCRE2_ALT_CIRCUMFLEX,	   // Alternative handling of ^ in multiline mode
		AltVerbNames	  = PCRE2_ALT_VERBNAMES,	   // Process backslashes in verb names
		AutoCallout		  = PCRE2_AUTO_CALLOUT,		   // Compile automatic callouts
		Caseless		  = PCRE2_CASELESS,			   // Do caseless matching
		DollarEndonly	  = PCRE2_DOLLAR_ENDONLY,	   // $ not to match newline at end
		DotAll			  = PCRE2_DOTALL,			   // . matches anything including NL
		DupNames		  = PCRE2_DUPNAMES,			   // Allow duplicate names for subpatterns
		EndAnchored		  = PCRE2_ENDANCHORED,		   // Pattern can match only at end of subject
		Extended		  = PCRE2_EXTENDED,			   // Ignore white space and # comments
		FirstLine		  = PCRE2_FIRSTLINE,		   // Force matching to be before newline
		Literal			  = PCRE2_LITERAL,			   // Pattern characters are all literal
		MatchInvalidUTF	  = PCRE2_MATCH_INVALID_UTF,   // Enable support for matching invalid UTF
		MatchUnsetBackRef = PCRE2_MATCH_UNSET_BACKREF, // Match unset backreferences
		Multiline		  = PCRE2_MULTILINE,		   // ^ and $ match newlines within data
		NeverBackslashC	  = PCRE2_NEVER_BACKSLASH_C,   // Lock out the use of \C in patterns
		NeverUCP		  = PCRE2_NEVER_UCP,		   // Lock out PCRE2_UCP, e.g.via(*UCP)
		NeverUTF		  = PCRE2_NEVER_UTF,		   // Lock out PCRE2_UTF, e.g.via(*UTF)
		NoAutoCapture	  = PCRE2_NO_AUTO_CAPTURE,	   // Disable numbered capturing paren - theses(named ones available)
		NoAutoPossess	  = PCRE2_NO_AUTO_POSSESS,	   // Disable auto - possessification
		NoDotStarAnchor	  = PCRE2_NO_DOTSTAR_ANCHOR,   // Disable automatic anchoring for .*
		NoStartOptimize	  = PCRE2_NO_START_OPTIMIZE,   // Disable match - time start optimizations
		NoUTFCheck		  = PCRE2_NO_UTF_CHECK, // Do not check the pattern for UTF validity (only relevant if PCRE2_UTF is set)
		UCP				  = PCRE2_UCP,			// Use Unicode properties for \d, \w, etc.
		UnGreedy		  = PCRE2_UNGREEDY,		// Invert greediness of quantifiers
		UseOffsetLimit	  = PCRE2_USE_OFFSET_LIMIT, // Enable offset limit for unanchored matching
		UTF				  = PCRE2_UTF				// Treat pattern and subjects as UTF strings
	};

	using compile_options = mstd::flags<compile_options_bits>;

	static _PCRE2CPP_CONSTEXPR17 compile_options operator|(const compile_options_bits opt0,
	  const compile_options_bits opt1) noexcept {
		return mstd::operator|(opt0, opt1);
	}
} // namespace pcre2cpp
	#endif
#endif