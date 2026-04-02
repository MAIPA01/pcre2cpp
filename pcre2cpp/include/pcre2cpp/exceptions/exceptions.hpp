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
	template<size_t utf>
	static _PCRE2CPP_CONSTEXPR17 typename pcre2_data<utf>::string_type generate_error_message(const int error_code) noexcept {
		using _pcre2_data_t = pcre2_data<utf>;
		using _string_type	= typename _pcre2_data_t::string_type;
		using _uchar_type	= typename _pcre2_data_t::uchar_type;
		using _char_type	= typename _pcre2_data_t::string_char_type;

		_uchar_type error_message[256];
			if (const int size = _pcre2_data_t::get_error_message(error_code, error_message, 256); size != PCRE2_ERROR_BADDATA) {
				return _string_type(reinterpret_cast<_char_type*>(error_message), 256);
			}
		return _string_type();
	}

	template<size_t utf>
	static _PCRE2CPP_CONSTEXPR17 typename pcre2_data<utf>::string_type generate_error_message(const int error_code,
	  const size_t error_offset) noexcept {
		using _string_type = typename pcre2_data<utf>::string_type;

			if _PCRE2CPP_CONSTEXPR17 (utf == 8) {
				return fmt::format("error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else if _PCRE2CPP_CONSTEXPR17 (utf == 16) {
				return fmt::format(u"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else if _PCRE2CPP_CONSTEXPR17 (utf == 32) {
				return fmt::format(U"error at {} {}", error_offset, pcre2cpp::generate_error_message<utf>(error_code));
			}
			else { return _string_type(); }
	}

		#if _PCRE2CPP_HAS_EXCEPTIONS
			#pragma region PCRE2CPP_EXCEPTION

	template<size_t utf>
	class basic_pcre2cpp_exception : public std::runtime_error {
	private:
		using _pcre2_data_t		= pcre2_data<utf>;
		using _string_type		= typename _pcre2_data_t::string_type;
		using _string_view_type = typename _pcre2_data_t::string_view_type;
		using _uchar_type		= typename _pcre2_data_t::uchar_type;
		using _char_type		= typename _pcre2_data_t::string_char_type;

		_string_type _message;

	public:
		explicit basic_pcre2cpp_exception(const _string_view_type message) noexcept
			: std::runtime_error(message.data()), _message(message) {}

		explicit basic_pcre2cpp_exception(const int error_code) noexcept
			: std::runtime_error(_generate_runtime_message(error_code)), _message(_generate_message(error_code)) {}

		basic_pcre2cpp_exception(const int error_code, const size_t error_offset) noexcept
			: std::runtime_error(_generate_runtime_message_with_offset(error_code, error_offset)),
			  _message(_generate_message_with_offset(error_code, error_offset)) {}

		_PCRE2CPP_CONSTEXPR17 const _string_type& get_error() const noexcept { return _message; }

	private:
		static _PCRE2CPP_CONSTEXPR20 std::string _generate_runtime_message(const int error_code) noexcept {
			return pcre2cpp::generate_error_message<8>(error_code);
		}

		static _PCRE2CPP_CONSTEXPR20 std::string _generate_runtime_message_with_offset(const int error_code,
		  const size_t error_offset) noexcept {
			return pcre2cpp::generate_error_message<8>(error_code, error_offset);
		}

		static _PCRE2CPP_CONSTEXPR17 _string_type _generate_message(const int error_code) noexcept {
			return pcre2cpp::generate_error_message<utf>(error_code);
		}

		static _PCRE2CPP_CONSTEXPR17 _string_type _generate_message_with_offset(const int error_code,
		  const size_t error_offset) noexcept {
			return pcre2cpp::generate_error_message<utf>(error_code, error_offset);
		}
	};

			#pragma endregion PCRE2CPP_EXCEPTION

			#pragma region REGEX_EXCEPTION

	template<size_t utf>
	class basic_regex_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_type = typename pcre2_data<utf>::string_type;

	public:
		basic_regex_exception(const int error_code, const size_t error_offset) noexcept
			: basic_pcre2cpp_exception<utf>(error_code, error_offset) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

			#pragma endregion REGEX_EXCEPTION

			#pragma region MATCH_RESULT_EXCEPTION

	template<size_t utf>
	class basic_match_result_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_type = typename pcre2_data<utf>::string_type;

	public:
		explicit basic_match_result_exception(const std::string_view message) noexcept : basic_pcre2cpp_exception<utf>(message) {}

		explicit basic_match_result_exception(const int error_code) noexcept : basic_pcre2cpp_exception<utf>(error_code) {}

		using basic_pcre2cpp_exception<utf>::get_error;
	};

			#pragma endregion MATCH_RESULT_EXCEPTION
		#endif
} // namespace pcre2cpp
	#endif
#endif