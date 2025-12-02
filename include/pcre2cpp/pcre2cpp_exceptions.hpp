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
#include "pcre2_data.hpp"

namespace pcre2cpp {
#ifdef PCRE2CPP_NO_EXCEPTIONS

	template<size_t utf>
	static constexpr _pcre2_data<utf>::string_type generate_error_message(int error_code) noexcept {
		using _string_type = _pcre2_data<utf>::string_type;
		using _uchar_type = _pcre2_data<utf>::uchar_type;
		using _char_type = _pcre2_data<utf>::string_char_type;

		_string_type message = _string_type();

		_uchar_type error_message[256];
		int size = _pcre2_data<utf>::get_error_message(error_code, error_message, 256);
		if (size != PCRE2_ERROR_BADDATA) {
			message.append(_string_type(reinterpret_cast<_char_type*>(error_message), 256));
		}
		return message;
	}

	template<size_t utf>
	static constexpr _pcre2_data<utf>::string_type generate_error_message(int error_code, size_t error_offset) noexcept {
		using _string_type = _pcre2_data<utf>::string_type;
		using _uchar_type = _pcre2_data<utf>::uchar_type;
		using _char_type = _pcre2_data<utf>::string_char_type;

		_string_type message = _string_type();

		if constexpr (utf == 8) {
			message
				.append("error at ")
				.append(std::to_string(error_offset))
				.append(" ");
		}
		else if constexpr (utf == 16) {
			message
				.append(L"error at ")
				.append(std::to_wstring(error_offset))
				.append(L" ");
		}
		else if constexpr (utf == 32) {
			_string_type value;
			for (wchar_t c : std::to_wstring(error_offset)) {
				value.append(1, c);
			}

			message
				.append(U"error at ")
				.append(value)
				.append(U" ");
		}
		return message.append(pcre2cpp::generate_error_message<utf>(error_code));
	}

#else
#pragma region PCRE2CPP_EXCEPTION
	template<size_t utf>
	class basic_pcre2cpp_exception : public std::runtime_error {
	private:
		using _string_type = _pcre2_data<utf>::string_type;
		using _uchar_type = _pcre2_data<utf>::uchar_type;
		using _char_type = _pcre2_data<utf>::string_char_type;

		_string_type _message;

	public:
		basic_pcre2cpp_exception(const std::string& message) noexcept
			: std::runtime_error(message), _message(message) {}
		basic_pcre2cpp_exception(int error_code) noexcept
			: std::runtime_error(_generate_runtime_message(error_code)),
			_message(_generate_message(error_code)) {}
		basic_pcre2cpp_exception(int error_code, size_t error_offset) noexcept
			: std::runtime_error(_generate_runtime_message_with_offset(error_code, error_offset)),
			_message(_generate_message_with_offset(error_code, error_offset)) {}

		constexpr _string_type get_error() const noexcept { 
			return _message;
		}

	private:
		static constexpr std::string _generate_runtime_message(int error_code) noexcept {
			uint8_t error_message[256];
			int size = _pcre2_data<8>::get_error_message(error_code, error_message, 256);

			std::string message = std::string();
			if (size != PCRE2_ERROR_BADDATA) {
				message += std::string(reinterpret_cast<char*>(error_message), 256);
			}
			return message;
		}

		static constexpr std::string _generate_runtime_message_with_offset(int error_code, size_t error_offset) noexcept {
			return std::string("error at ")
				.append(std::to_string(error_offset))
				.append(" ")
				.append(_generate_runtime_message(error_code));
		}

		static constexpr _string_type _generate_message(int error_code) noexcept {
			_string_type message = _string_type();

			_uchar_type error_message[256];
			int size = _pcre2_data<utf>::get_error_message(error_code, error_message, 256);
			if (size != PCRE2_ERROR_BADDATA) {
				message.append(_string_type(reinterpret_cast<_char_type*>(error_message), 256));
			}
			return message;
		}

		static constexpr _string_type _generate_message_with_offset(int error_code, size_t error_offset) noexcept {
			_string_type message = _string_type();

			if constexpr (utf == 8) {
				message
					.append("error at ")
					.append(std::to_string(error_offset))
					.append(" ");
			}
			else if constexpr (utf == 16) {
				message
					.append(L"error at ")
					.append(std::to_wstring(error_offset))
					.append(L" ");
			}
			else if constexpr (utf == 32) {
				std::u32string value;
				for (wchar_t c : std::to_wstring(error_offset)) {
					value.append(1, c);
				}
				message
					.append(U"error at ")
					.append(value)
					.append(U" ");
			}
			return message.append(_generate_message(error_code));
		}
	};
#pragma endregion PCRE2CPP_EXCEPTION

#pragma region REGEX_EXCEPTION
	template<size_t utf>
	class basic_regex_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_type = _pcre2_data<utf>::string_type;

	public:
		basic_regex_exception(int error_code, size_t error_offset) noexcept
			: basic_pcre2cpp_exception<utf>(error_code, error_offset) {}

		constexpr _string_type get_error() const noexcept {
			return basic_pcre2cpp_exception<utf>::get_error();
		}
	};
#pragma endregion REGEX_EXCEPTION

#pragma region MATCH_RESULT_EXCEPTION
	template<size_t utf>
	class basic_match_result_exception : public basic_pcre2cpp_exception<utf> {
	private:
		using _string_type = _pcre2_data<utf>::string_type;

	public:
		basic_match_result_exception(const std::string& message) noexcept
			: basic_pcre2cpp_exception<utf>(message) {}
		basic_match_result_exception(int error_code) noexcept
			: basic_pcre2cpp_exception<utf>(error_code) {}

		constexpr _string_type get_error() const noexcept { 
			return basic_pcre2cpp_exception<utf>::get_error();
		}
	};
#pragma endregion MATCH_RESULT_EXCEPTION
#endif
}