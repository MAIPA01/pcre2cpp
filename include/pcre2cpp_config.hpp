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
#define PCRE2CPP_VERSION_MINOR 0
#define PCRE2CPP_VERSION_PATCH 0

#define PCRE2CPP_VERSION_TO_STRING(major, minor, patch) #major #minor #patch
#define PCRE2CPP_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define PCRE2CPP_VERSION_STRING PCRE2CPP_VERSION_TO_STRING(PCRE2CPP_VERSION_MAJOR, \
															PCRE2CPP_VERSION_MINOR, \
															PCRE2CPP_VERSION_PATCH)
#define PCRE2CPP_VERSION_INT PCRE2CPP_VERSION_TO_INT(PCRE2CPP_VERSION_MAJOR, \
													PCRE2CPP_VERSION_MINOR, \
													PCRE2CPP_VERSION_PATCH)
#define PCRE2CPP_VERSION PCRE2CPP_VERSION_STRING
#pragma endregion

#pragma region LAST_UPDATE
#define PCRE2CPP_LAST_UPDATE_DAY 12
#define PCRE2CPP_LAST_UPDATE_MONTH 3
#define PCRE2CPP_LAST_UPDATE_YEAR 2025

#define PCRE2CPP_LAST_UPDATE_DATE "12.03.2025"
#pragma endregion