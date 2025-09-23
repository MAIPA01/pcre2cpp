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
	template<size_t utf> 
	struct _pcre2_data {};

#pragma region UTF-8
	template<>
	struct _pcre2_data<8> {
		using code_type = pcre2_code_8;
		using string_ptr_type = PCRE2_SPTR8;
		using compile_ctx_type = pcre2_compile_context_8;
		using general_ctx_type = pcre2_general_context_8;
		using match_ctx_type = pcre2_match_context_8;
		using match_data_type = pcre2_match_data_8;
		using string_type = std::string;
		using string_char_type = char;
		using uchar_type = PCRE2_UCHAR8;

		static constexpr size_t utf = 8;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_8;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_8;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_8;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_8;
		static constexpr uint32_t(*get_ovector_count)(match_data_type*) = pcre2_get_ovector_count_8;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_8;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_8;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_8;
		static constexpr int(*get_info)(const code_type*, uint32_t, void*) = pcre2_pattern_info_8;
		static constexpr int(*substring_number_from_name)(const code_type*, string_ptr_type) = pcre2_substring_number_from_name_8;
	};
#pragma endregion UTF-8

#pragma region UTF-16
	template<>
	struct _pcre2_data<16> {
		using code_type = pcre2_code_16;
		using string_ptr_type = PCRE2_SPTR16;
		using compile_ctx_type = pcre2_compile_context_16;
		using general_ctx_type = pcre2_general_context_16;
		using match_ctx_type = pcre2_match_context_16;
		using match_data_type = pcre2_match_data_16;
		using string_type = std::wstring;
		using string_char_type = wchar_t;
		using uchar_type = PCRE2_UCHAR16;

		static constexpr size_t utf = 16;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_16;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_16;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_16;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_16;
		static constexpr uint32_t(*get_ovector_count)(match_data_type*) = pcre2_get_ovector_count_16;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_16;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_16;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_16;
		static constexpr int(*get_info)(const code_type*, uint32_t, void*) = pcre2_pattern_info_16;
		static constexpr int(*substring_number_from_name)(const code_type*, string_ptr_type) = pcre2_substring_number_from_name_16;
	};
#pragma endregion UTF-16

#pragma region UTF-32
	template<>
	struct _pcre2_data<32> {
		using code_type = pcre2_code_32;
		using string_ptr_type = PCRE2_SPTR32;
		using compile_ctx_type = pcre2_compile_context_32;
		using general_ctx_type = pcre2_general_context_32;
		using match_ctx_type = pcre2_match_context_32;
		using match_data_type = pcre2_match_data_32;
		using string_type = std::u32string;
		using string_char_type = char32_t;
		using uchar_type = PCRE2_UCHAR32;

		static constexpr size_t utf = 32;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_32;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_32;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_32;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_32;
		static constexpr uint32_t(*get_ovector_count)(match_data_type*) = pcre2_get_ovector_count_32;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_32;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_32;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_32;
		static constexpr int(*get_info)(const code_type*, uint32_t, void*) = pcre2_pattern_info_32;
		static constexpr int(*substring_number_from_name)(const code_type*, string_ptr_type) = pcre2_substring_number_from_name_32;
	};
#pragma endregion UTF-32
}