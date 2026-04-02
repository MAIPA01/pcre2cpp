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
#ifndef _PCRE2CPP_PCRE2_DATA_HPP_
	#define _PCRE2CPP_PCRE2_DATA_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else
		#include <pcre2cpp/types.hpp>

namespace pcre2cpp {
	template<size_t utf>
	struct pcre2_data {};

		#pragma region UTF-8

	template<>
	struct pcre2_data<8> {
		// CODE
		using code_type							= pcre2_code_8;
		using compile_ctx_type					= pcre2_compile_context_8;
		using general_ctx_type					= pcre2_general_context_8;

		// MATCH
		using match_data_type					= pcre2_match_data_8;
		using match_ctx_type					= pcre2_match_context_8;

		// PCRE2 STRING
		using sptr_type							= PCRE2_SPTR8;
		using uchar_type						= PCRE2_UCHAR8;

		// CPP STRING
		// #if _PCRE2CPP_HAS_CXX20
		// using string_type						= std::u8string;
		// using string_view_type					= std::u8string_view;
		// #else
		// using string_type						= std::string;
		// using string_view_type					= std::string_view;
		// #endif
		using string_type						= std::string;
		using string_view_type					= std::string_view;
		using string_char_type					= string_type::value_type;

		static _PCRE2CPP_CONSTEXPR17 size_t utf = 8;

		// FUNCTIONS
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_8;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_8;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_8;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_8;

		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_8;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_8;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_8;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_8;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_8;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_8;
	};

		#pragma endregion

		#pragma region UTF-16

	template<>
	struct pcre2_data<16> {
		// CODE
		using code_type							= pcre2_code_16;
		using compile_ctx_type					= pcre2_compile_context_16;
		using general_ctx_type					= pcre2_general_context_16;

		// MATCH
		using match_data_type					= pcre2_match_data_16;
		using match_ctx_type					= pcre2_match_context_16;

		// PCRE2 STRING
		using sptr_type							= PCRE2_SPTR16;
		using uchar_type						= PCRE2_UCHAR16;

		// CPP STRING
		using string_type						= std::u16string;
		using string_view_type					= std::u16string_view;
		using string_char_type					= string_type::value_type;

		static _PCRE2CPP_CONSTEXPR17 size_t utf = 16;

		// FUNCTIONS
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_16;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_16;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_16;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_16;

		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_16;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_16;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_16;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_16;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_16;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_16;
	};

		#pragma endregion

		#pragma region UTF-32

	template<>
	struct pcre2_data<32> {
		// CODE
		using code_type							= pcre2_code_32;
		using compile_ctx_type					= pcre2_compile_context_32;
		using general_ctx_type					= pcre2_general_context_32;

		// MATCH
		using match_data_type					= pcre2_match_data_32;
		using match_ctx_type					= pcre2_match_context_32;

		// PCRE2 STRING
		using sptr_type							= PCRE2_SPTR32;
		using uchar_type						= PCRE2_UCHAR32;

		// CPP STRING
		using string_type						= std::u32string;
		using string_view_type					= std::u32string_view;
		using string_char_type					= string_type::value_type;

		static _PCRE2CPP_CONSTEXPR17 size_t utf = 32;

		// FUNCTIONS
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_32;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_32;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_32;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_32;

		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_32;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_32;
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_32;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_32;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_32;

		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_32;
	};

		#pragma endregion
} // namespace pcre2cpp
	#endif
#endif