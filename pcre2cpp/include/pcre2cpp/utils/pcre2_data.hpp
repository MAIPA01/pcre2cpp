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
	/**
	 * @brief Enum with supported utf types
	 * @ingroup pcre2cpp
	 */
	enum class utf_type : uint8_t {
		#if _PCRE2CPP_HAS_UTF8
		UTF_8 = 8,
		#endif
		#if _PCRE2CPP_HAS_UTF16
		UTF_16 = 16,
		#endif
		#if _PCRE2CPP_HAS_UTF32
		UTF_32 = 32
		#endif
	};
} // namespace pcre2cpp

namespace pcre2cpp::utils {
	/**
	 * @brief Translation container from pcre2 library to pcre2cpp
	 * @ingroup utils
	 * @tparam utf utf coding type (UTF-8 -> utf_type::UTF_8, UTF-16 -> utf_type::UTF_16, UTF-32 -> utf_type::UTF_32)
	 */
	template<utf_type utf>
	struct pcre2_data {};

		#pragma region UTF_8
		#if _PCRE2CPP_HAS_UTF8
	/**
	 * @brief Specialization of Translation container from pcre2 library to pcre2cpp for UTF-8
	 * @ingroup utils
	 */
	template<>
	struct pcre2_data<utf_type::UTF_8> {
			#pragma region CODE
		/// @brief pcre2 code structure type for utf-8
		using code_type								 = pcre2_code_8;
		/// @brief pcre2 compile context structure type for utf-8
		using compile_ctx_type						 = pcre2_compile_context_8;
		/// @brief pcre2 general context structure type for utf-8
		using general_ctx_type						 = pcre2_general_context_8;
			#pragma endregion

			#pragma region MATCH
		/// @brief pcre2 match data structure type for utf-8
		using match_data_type						 = pcre2_match_data_8;
		/// @brief pcre2 match context structure type for utf-8
		using match_ctx_type						 = pcre2_match_context_8;
			#pragma endregion

			#pragma region PCRE2_STRING
		/// @brief pcre2 string pointer type for utf-8
		using sptr_type								 = PCRE2_SPTR8;
		/// @brief pcre2 unsigned char type for utf-8
		using uchar_type							 = PCRE2_UCHAR8;
			#pragma endregion

			#pragma region CPP_STRING
		// #if _PCRE2CPP_HAS_CXX20
		// using string_type						= std::u8string;
		// using string_view_type					= std::u8string_view;
		// #else
		// using string_type						= std::string;
		// using string_view_type					= std::string_view;
		// #endif
		/// @brief cpp string type for utf-8
		using string_type							 = std::string;
		/// @brief cpp string view type for utf-8
		using string_view_type						 = std::string_view;
		/// @brief cpp string char type for utf-8
		using string_char_type						 = string_type::value_type;
			#pragma endregion

			#pragma region UTF_INFO
		/// @brief utf enum value for utf-8
		static _PCRE2CPP_CONSTEXPR17 utf_type uft	 = utf_type::UTF_8;
		/// @brief utf byte size for utf-8
		static _PCRE2CPP_CONSTEXPR17 size_t utf_size = 8;
			#pragma endregion

			#pragma region CODE_FUNCTIONS
		/// @brief pointer to pcre2_compile function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_8;
		/// @brief pointer to pcre2_code_free function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_8;
			#pragma endregion

			#pragma region MATCH_DATA_FUNCTIONS
		/// @brief pointer to pcre2_match_data_create_from_pattern function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_8;
		/// @brief pointer to pcre2_match_data_free function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_8;
			#pragma endregion

			#pragma region MATCH_FUNCTIONS
		/// @brief pointer to pcre2_match function for utf-8
		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_8;
			#pragma endregion

			#pragma region OVECTOR_FUNCTIONS
		/// @brief pointer to pcre2_get_ovector_pointer function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_8;
		/// @brief pointer to pcre2_get_ovector_count function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_8;
			#pragma endregion

			#pragma region ERROR_FUNCTIONS
		/// @brief pointer to pcre2_get_error_message function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_8;
			#pragma endregion

			#pragma region PATTERN_INFO_FUNCTIONS
		/// @brief pointer to pcre2_pattern_info function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_8;
			#pragma endregion

			#pragma region SUBSTRING_FUNCTIONS
		/// @brief pointer to pcre2_substring_number_from_name function for utf-8
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_8;
			#pragma endregion
	};
		#endif

		#pragma endregion

		#pragma region UTF_16

		#if _PCRE2CPP_HAS_UTF16
	/**
	 * @brief Specialization of Translation container from pcre2 library to pcre2cpp for UTF-16
	 * @ingroup utils
	 */
	template<>
	struct pcre2_data<utf_type::UTF_16> {
			#pragma region CODE
		/// @brief pcre2 code structure type for utf-16
		using code_type								 = pcre2_code_16;
		/// @brief pcre2 compile context structure type for utf-16
		using compile_ctx_type						 = pcre2_compile_context_16;
		/// @brief pcre2 general context structure type for utf-16
		using general_ctx_type						 = pcre2_general_context_16;
			#pragma endregion

			#pragma region MATCH
		/// @brief pcre2 match data structure type for utf-16
		using match_data_type						 = pcre2_match_data_16;
		/// @brief pcre2 match context structure type for utf-16
		using match_ctx_type						 = pcre2_match_context_16;
			#pragma endregion

			#pragma region PCRE2_STRING
		/// @brief pcre2 string pointer type for utf-16
		using sptr_type								 = PCRE2_SPTR16;
		/// @brief pcre2 unsigned char type for utf-16
		using uchar_type							 = PCRE2_UCHAR16;
			#pragma endregion

			#pragma region CPP_STRING
		/// @brief cpp string type for utf-16
		using string_type							 = std::u16string;
		/// @brief cpp string view type for utf-16
		using string_view_type						 = std::u16string_view;
		/// @brief cpp string char type for utf-16
		using string_char_type						 = string_type::value_type;
			#pragma endregion

			#pragma region UTF_INFO
		/// @brief utf enum value for utf-16
		static _PCRE2CPP_CONSTEXPR17 utf_type uft	 = utf_type::UTF_16;
		/// @brief utf byte size for utf-16
		static _PCRE2CPP_CONSTEXPR17 size_t utf_size = 16;
			#pragma endregion

			#pragma region CODE_FUNCTIONS
		/// @brief pointer to pcre2_compile function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_16;
		/// @brief pointer to pcre2_code_free function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_16;
			#pragma endregion

			#pragma region MATCH_DATA_FUNCTIONS
		/// @brief pointer to pcre2_match_data_create_from_pattern function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_16;
		/// @brief pointer to pcre2_match_data_free function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_16;
			#pragma endregion

			#pragma region MATCH_FUNCTIONS
		/// @brief pointer to pcre2_match function for utf-16
		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_16;
			#pragma endregion

			#pragma region OVECTOR_FUNCTIONS
		/// @brief pointer to pcre2_get_ovector_pointer function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_16;
		/// @brief pointer to pcre2_get_ovector_count function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_16;
			#pragma endregion

			#pragma region ERROR_FUNCTIONS
		/// @brief pointer to pcre2_get_error_message function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_16;
			#pragma endregion

			#pragma region PATTERN_INFO_FUNCTIONS
		/// @brief pointer to pcre2_pattern_info function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_16;
			#pragma endregion

			#pragma region SUBSTRING_FUNCTIONS
		/// @brief pointer to pcre2_substring_number_from_name function for utf-16
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_16;
			#pragma endregion
	};
		#endif

		#pragma endregion

		#pragma region UTF_32

		#if _PCRE2CPP_HAS_UTF32
	/**
	 * @brief Specialization of Translation container from pcre2 library to pcre2cpp for UTF-32
	 * @ingroup utils
	 */
	template<>
	struct pcre2_data<utf_type::UTF_32> {
			#pragma region CODE
		/// @brief pcre2 code structure type for utf-32
		using code_type								 = pcre2_code_32;
		/// @brief pcre2 compile context structure type for utf-32
		using compile_ctx_type						 = pcre2_compile_context_32;
		/// @brief pcre2 general context structure type for utf-32
		using general_ctx_type						 = pcre2_general_context_32;
			#pragma endregion

			#pragma region MATCH
		/// @brief pcre2 match data structure type for utf-32
		using match_data_type						 = pcre2_match_data_32;
		/// @brief pcre2 match context structure type for utf-32
		using match_ctx_type						 = pcre2_match_context_32;
			#pragma endregion

			#pragma region PCRE2_STRING
		/// @brief pcre2 string pointer type for utf-32
		using sptr_type								 = PCRE2_SPTR32;
		/// @brief pcre2 unsigned char type for utf-32
		using uchar_type							 = PCRE2_UCHAR32;
			#pragma endregion

			#pragma region CPP_STRING
		/// @brief cpp string type for utf-32
		using string_type							 = std::u32string;
		/// @brief cpp string view type for utf-32
		using string_view_type						 = std::u32string_view;
		/// @brief cpp string char type for utf-32
		using string_char_type						 = string_type::value_type;
			#pragma endregion

			#pragma region UTF_INFO
		/// @brief utf enum value for utf-32
		static _PCRE2CPP_CONSTEXPR17 utf_type uft	 = utf_type::UTF_32;
		/// @brief utf byte size for utf-32
		static _PCRE2CPP_CONSTEXPR17 size_t utf_size = 32;
			#pragma endregion

			#pragma region CODE_FUNCTIONS
		/// @brief pointer to pcre2_compile function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<code_type*(sptr_type, size_t, uint32_t, int*, size_t*, compile_ctx_type*)>
		  compile																= pcre2_compile_32;
		/// @brief pointer to pcre2_code_free function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(code_type*)> code_free = pcre2_code_free_32;
			#pragma endregion

			#pragma region MATCH_DATA_FUNCTIONS
		/// @brief pointer to pcre2_match_data_create_from_pattern function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<match_data_type*(const code_type*, general_ctx_type*)>
		  match_data_from_pattern = pcre2_match_data_create_from_pattern_32;
		/// @brief pointer to pcre2_match_data_free function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<void(match_data_type*)> match_data_free = pcre2_match_data_free_32;
			#pragma endregion

			#pragma region MATCH_FUNCTIONS
		/// @brief pointer to pcre2_match function for utf-32
		static _PCRE2CPP_CONSTEXPR17
		  mstd::c_func_t<int(const code_type*, sptr_type, size_t, size_t, uint32_t, match_data_type*, match_ctx_type*)>
			match																				  = pcre2_match_32;
			#pragma endregion

			#pragma region OVECTOR_FUNCTIONS
		/// @brief pointer to pcre2_get_ovector_pointer function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<size_t*(match_data_type*)> get_ovector_ptr	  = pcre2_get_ovector_pointer_32;
		/// @brief pointer to pcre2_get_ovector_count function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<uint32_t(match_data_type*)> get_ovector_count = pcre2_get_ovector_count_32;
			#pragma endregion

			#pragma region ERROR_FUNCTIONS
		/// @brief pointer to pcre2_get_error_message function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(int, uchar_type*, size_t)> get_error_message = pcre2_get_error_message_32;
			#pragma endregion

			#pragma region PATTERN_INFO_FUNCTIONS
		/// @brief pointer to pcre2_pattern_info function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, uint32_t, void*)> get_info = pcre2_pattern_info_32;
			#pragma endregion

			#pragma region SUBSTRING_FUNCTIONS
		/// @brief pointer to pcre2_substring_number_from_name function for utf-32
		static _PCRE2CPP_CONSTEXPR17 mstd::c_func_t<int(const code_type*, sptr_type)> substring_number_from_name =
		  pcre2_substring_number_from_name_32;
			#pragma endregion
	};
		#endif

		#pragma endregion

		#if _PCRE2CPP_HAS_UTF8
	using u8pcre2_data = pcre2_data<utf_type::UTF_8>;
		#endif
		#if _PCRE2CPP_HAS_UTF16
	using u16pcre2_data = pcre2_data<utf_type::UTF_16>;
		#endif
		#if _PCRE2CPP_HAS_UTF32
	using u32pcre2_data = pcre2_data<utf_type::UTF_32>;
		#endif
} // namespace pcre2cpp::utils
	#endif
#endif