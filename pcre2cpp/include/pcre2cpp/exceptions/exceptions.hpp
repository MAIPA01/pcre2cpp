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
#ifndef _PCRE2CPP_EXCEPTIONS_HPP_
	#define _PCRE2CPP_EXCEPTIONS_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/types.hpp>
		#include <pcre2cpp/utils/pcre2_data.hpp>
		#if _PCRE2CPP_HAS_ASSERTS
			#include <pcre2cpp/utils/assert.hpp>
		#endif

namespace pcre2cpp {
	/**
	 * @brief Function which generate error message based on pcre2 error code
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 * @param error_code pcre2 error code
	 * @return string value of type compatible with UTF type
	 */
	template<utf_type utf>
	static _PCRE2CPP_CONSTEXPR17 typename utils::pcre2_data<utf>::string_type generate_error_message(
	  const int error_code
	) noexcept {
		using _pcre2_data_t = utils::pcre2_data<utf>;
		using _string_type	= typename _pcre2_data_t::string_type;
		using _uchar_type	= typename _pcre2_data_t::uchar_type;
		using _char_type	= typename _pcre2_data_t::string_char_type;

		_uchar_type error_message[120];
			if (const int size = _pcre2_data_t::get_error_message(error_code, error_message, 120); size != PCRE2_ERROR_BADDATA) {
				return _string_type(reinterpret_cast<_char_type*>(error_message), 120);
			}
		return _string_type();
	}

	/**
	 * @brief Function which generates error message based on pcre2 error code and with additional information about offset
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 * @param error_code pcre2 error code
	 * @param error_offset offset for example in regex pattern
	 * @return string value of type compatible with UTF type
	 */
	template<utf_type utf>
	static _PCRE2CPP_CONSTEXPR17 typename utils::pcre2_data<utf>::string_type generate_error_message(const int error_code,
	  const size_t error_offset) noexcept {
		using _string_type = typename utils::pcre2_data<utf>::string_type;

		#if _PCRE2CPP_HAS_UTF8
			if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) {
				return fmt::format("error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else
		#endif
		#if _PCRE2CPP_HAS_UTF16
			  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) {
				return fmt::format(u"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else
		#endif
		#if _PCRE2CPP_HAS_UTF32
			  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) {
				return fmt::format(U"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else
		#endif
			{
				return _string_type();
			}
	}

	/**
	 * @brief converts any message from any utf to utf-8
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 * @param message message to convert
	 * @return std::string value of message
	 */
	template<utf_type utf>
	static _PCRE2CPP_CONSTEXPR20 std::string convert_any_utf_to_utf8(const typename utils::pcre2_data<utf>::string_view_type message) noexcept {
		#if _PCRE2CPP_HAS_UTF8
		if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) { return std::string(message); }
		else
			#endif
			#if _PCRE2CPP_HAS_UTF16
			if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) {
			std::string msg;
			for (const auto& c : message) { msg += static_cast<std::string::value_type>(c); }
			return msg;
		}
		else
			#endif
			#if _PCRE2CPP_HAS_UTF32
			if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) {
			std::string msg;
			for (const auto& c : message) { msg += static_cast<std::string::value_type>(c); }
			return msg;
		}
		else
			#endif
		{
			return std::string();
		}
	}

		#if _PCRE2CPP_HAS_EXCEPTIONS
			#pragma region PCRE2CPP_EXCEPTION

	/**
	 * @brief base pcre2cpp exception class
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	class basic_pcre2cpp_exception : public std::runtime_error {
	private:
		using _pcre2_data_t		= utils::pcre2_data<utf>;
		using _string_type		= typename _pcre2_data_t::string_type;
		using _string_view_type = typename _pcre2_data_t::string_view_type;
		using _uchar_type		= typename _pcre2_data_t::uchar_type;
		using _char_type		= typename _pcre2_data_t::string_char_type;

		/// @brief error message
		_string_type _message;

	public:
		/// @brief constructor with message
		explicit basic_pcre2cpp_exception(const _string_view_type message) noexcept
			: std::runtime_error(convert_any_utf_to_utf8<utf>(message)), _message(message) {}

		/// @brief constructor with error code
		explicit basic_pcre2cpp_exception(const int error_code) noexcept
			: std::runtime_error(convert_any_utf_to_utf8<utf>(generate_error_message<utf>(error_code))),
			  _message(generate_error_message<utf>(error_code)) {}

		/// @brief constructor with error code and error offset
		basic_pcre2cpp_exception(const int error_code, const size_t error_offset) noexcept
			: std::runtime_error(convert_any_utf_to_utf8<utf>(generate_error_message<utf>(error_code, error_offset))),
			  _message(generate_error_message<utf>(error_code, error_offset)) {}

		/// @brief returns error message
		_PCRE2CPP_CONSTEXPR17 const _string_type& get_error() const noexcept { return _message; }
	};

			#if _PCRE2CPP_HAS_UTF8
	using u8pcre2cpp_exception = basic_pcre2cpp_exception<utf_type::UTF_8>;
			#endif
			#if _PCRE2CPP_HAS_UTF16
	using u16pcre2cpp_exception = basic_pcre2cpp_exception<utf_type::UTF_16>;
			#endif
			#if _PCRE2CPP_HAS_UTF32
	using u32pcre2cpp_exception = basic_pcre2cpp_exception<utf_type::UTF_32>;
			#endif

	using pcre2cpp_exception = basic_pcre2cpp_exception<default_utf_type>;

			#pragma endregion PCRE2CPP_EXCEPTION

			#pragma region REGEX_EXCEPTION

	/**
	 * @brief regex exception class
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	class basic_regex_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_view_type = typename utils::pcre2_data<utf>::string_view_type;

	public:
		/// @brief constructor with message
		explicit basic_regex_exception(const _string_view_type message) noexcept : basic_pcre2cpp_exception<utf>(message) {}

		/// @brief constructor with error code and error offset
		basic_regex_exception(const int error_code, const size_t error_offset) noexcept
			: basic_pcre2cpp_exception<utf>(error_code, error_offset) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

			#if _PCRE2CPP_HAS_UTF8
	using u8regex_exception = basic_regex_exception<utf_type::UTF_8>;
			#endif
			#if _PCRE2CPP_HAS_UTF16
	using u16regex_exception = basic_regex_exception<utf_type::UTF_16>;
			#endif
			#if _PCRE2CPP_HAS_UTF32
	using u32regex_exception = basic_regex_exception<utf_type::UTF_32>;
			#endif

	using regex_exception = basic_regex_exception<default_utf_type>;

			#pragma endregion REGEX_EXCEPTION

			#pragma region MATCH_RESULT_EXCEPTION

	/**
	 * @brief match result exception class
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	class basic_match_result_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_view_type = typename utils::pcre2_data<utf>::string_view_type;

	public:
		/// @brief constructor with message
		explicit basic_match_result_exception(const _string_view_type message) noexcept
			: basic_pcre2cpp_exception<utf>(message) {}

		/// @brief constructor with error code
		explicit basic_match_result_exception(const int error_code) noexcept : basic_pcre2cpp_exception<utf>(error_code) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

			#if _PCRE2CPP_HAS_UTF8
	using u8match_result_exception = basic_match_result_exception<utf_type::UTF_8>;
			#endif
			#if _PCRE2CPP_HAS_UTF16
	using u16match_result_exception = basic_match_result_exception<utf_type::UTF_16>;
			#endif
			#if _PCRE2CPP_HAS_UTF32
	using u32match_result_exception = basic_match_result_exception<utf_type::UTF_32>;
			#endif

	using match_result_exception = basic_match_result_exception<default_utf_type>;

			#pragma endregion MATCH_RESULT_EXCEPTION
		#endif
} // namespace pcre2cpp
	#endif
#endif