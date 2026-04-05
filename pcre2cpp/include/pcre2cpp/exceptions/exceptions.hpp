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
	static _PCRE2CPP_CONSTEXPR17 typename utils::pcre2_data<utf>::string_type generate_error_message(const int error_code) noexcept {
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

			if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) {
				return fmt::format("error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) {
				return fmt::format(u"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) {
				return fmt::format(U"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else { return _string_type(); }
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
			: std::runtime_error(_convert_error_message_to_runtime(message)), _message(message) {}

		/// @brief constructor with error code
		explicit basic_pcre2cpp_exception(const int error_code) noexcept
			: std::runtime_error(_generate_runtime_message(error_code)), _message(_generate_message(error_code)) {}

		/// @brief constructor with error code and error offset
		basic_pcre2cpp_exception(const int error_code, const size_t error_offset) noexcept
			: std::runtime_error(_generate_runtime_message_with_offset(error_code, error_offset)),
			  _message(_generate_message_with_offset(error_code, error_offset)) {}

		/// @breif returns error message
		_PCRE2CPP_CONSTEXPR17 const _string_type& get_error() const noexcept { return _message; }

	private:
		/// @brief converts any message from any utf to utf-8
		static _PCRE2CPP_CONSTEXPR20 std::string _convert_error_message_to_runtime(const _string_view_type message) noexcept {
			if _PCRE2CPP_CONSTEXPR17 (utf == pcre2cpp::utf_type::UTF_8) {
				return std::string(message);
			}
			else if _PCRE2CPP_CONSTEXPR17 (utf == pcre2cpp::utf_type::UTF_16 || utf == pcre2cpp::utf_type::UTF_32) {
				std::string msg;
				for (const auto& c : message) {
					msg += static_cast<std::string::value_type>(c);
				}
				return msg;
			}
			else {
				return std::string();
			}
		}

		/// @brief generates runtime message based on error code
		static _PCRE2CPP_CONSTEXPR20 std::string _generate_runtime_message(const int error_code) noexcept {
			return generate_error_message<utf_type::UTF_8>(error_code);
		}

		/// @brief generates runtime message based on error code and error offset
		static _PCRE2CPP_CONSTEXPR20 std::string _generate_runtime_message_with_offset(const int error_code,
		  const size_t error_offset) noexcept {
			return generate_error_message<utf_type::UTF_8>(error_code, error_offset);
		}

		/// @brief generates message in correct UTF format based on error code
		static _PCRE2CPP_CONSTEXPR17 _string_type _generate_message(const int error_code) noexcept {
			return generate_error_message<utf>(error_code);
		}

		/// @brief generates message in correct UTF format based on error code and error offset
		static _PCRE2CPP_CONSTEXPR17 _string_type _generate_message_with_offset(const int error_code,
		  const size_t error_offset) noexcept {
			return generate_error_message<utf>(error_code, error_offset);
		}
	};

	using u8pcre2cpp_exception		= basic_pcre2cpp_exception<utf_type::UTF_8>;
	using u16pcre2cpp_exception		= basic_pcre2cpp_exception<utf_type::UTF_16>;
	using u32pcre2cpp_exception		= basic_pcre2cpp_exception<utf_type::UTF_32>;
	using pcre2cpp_exception		= u8pcre2cpp_exception;

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
		explicit basic_regex_exception(const _string_view_type message) noexcept
			: basic_pcre2cpp_exception<utf>(message) {}

		/// @brief constructor with error code and error offset
		basic_regex_exception(const int error_code, const size_t error_offset) noexcept
			: basic_pcre2cpp_exception<utf>(error_code, error_offset) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

	using u8regex_exception			= basic_regex_exception<utf_type::UTF_8>;
	using u16regex_exception		= basic_regex_exception<utf_type::UTF_16>;
	using u32regex_exception		= basic_regex_exception<utf_type::UTF_32>;
	using regex_exception			= u8regex_exception;

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
		explicit basic_match_result_exception(const _string_view_type message) noexcept : basic_pcre2cpp_exception<utf>(message) {}

		/// @brief constructor with error code
		explicit basic_match_result_exception(const int error_code) noexcept : basic_pcre2cpp_exception<utf>(error_code) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

	using u8match_result_exception	= basic_match_result_exception<utf_type::UTF_8>;
	using u16match_result_exception = basic_match_result_exception<utf_type::UTF_16>;
	using u32match_result_exception = basic_match_result_exception<utf_type::UTF_32>;
	using match_result_exception	= u8match_result_exception;

			#pragma endregion MATCH_RESULT_EXCEPTION
		#endif
} // namespace pcre2cpp
	#endif
#endif