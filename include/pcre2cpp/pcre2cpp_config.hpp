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

#pragma region VERSION
#define PCRE2CPP_VERSION_MAJOR 1
#define PCRE2CPP_VERSION_MINOR 1
#define PCRE2CPP_VERSION_PATCH 0

#define _PCRE2CPP_STRINGIFY_HELPER(x) #x

#define _PCRE2CPP_VERSION_TO_STRING(major, minor, patch) _PCRE2CPP_STRINGIFY_HELPER(major)"."\
														_PCRE2CPP_STRINGIFY_HELPER(minor)"."\
														_PCRE2CPP_STRINGIFY_HELPER(patch)
#define _PCRE2CPP_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define PCRE2CPP_VERSION_STRING _PCRE2CPP_VERSION_TO_STRING(PCRE2CPP_VERSION_MAJOR, \
															PCRE2CPP_VERSION_MINOR, \
															PCRE2CPP_VERSION_PATCH)
#define PCRE2CPP_VERSION_INT _PCRE2CPP_VERSION_TO_INT(PCRE2CPP_VERSION_MAJOR, \
													PCRE2CPP_VERSION_MINOR, \
													PCRE2CPP_VERSION_PATCH)
#define PCRE2CPP_VERSION PCRE2CPP_VERSION_STRING
#pragma endregion VERSION

#pragma region LAST_UPDATE
#define PCRE2CPP_LAST_UPDATE_DAY 23
#define PCRE2CPP_LAST_UPDATE_MONTH 9
#define PCRE2CPP_LAST_UPDATE_YEAR 2025

#define _PCRE2CPP_LAST_UPDATE_DATE_HELPER(day, month, year) _PCRE2CPP_STRINGIFY_HELPER(day)"."\
															_PCRE2CPP_STRINGIFY_HELPER(month)"."\
															_PCRE2CPP_STRINGIFY_HELPER(year)

#define PCRE2CPP_LAST_UPDATE_DATE _PCRE2CPP_LAST_UPDATE_DATE_HELPER(PCRE2CPP_LAST_UPDATE_DAY, \
																	PCRE2CPP_LAST_UPDATE_MONTH, \
																	PCRE2CPP_LAST_UPDATE_YEAR)
#pragma endregion LAST_UPDATE