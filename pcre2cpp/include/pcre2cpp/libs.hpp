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
#ifndef _PCRE2CPP_LIBS_HPP_
	#define _PCRE2CPP_LIBS_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2.h>
		#include <string>
		#if _PCRE2CPP_HAS_EXCEPTIONS
			#include <stdexcept>
		#endif
		#include <fmt/format.h>
		#include <fmt/xchar.h>
		#include <memory>
		#include <mstd/mstd.hpp>
		#include <unordered_map>
		#include <variant>
		#include <vector>

	#endif
#endif