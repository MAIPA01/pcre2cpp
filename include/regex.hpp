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
	template<size_t utf>
	class basic_regex {
	private:
		using _code_type = _pcre2_data<utf>::code_type;
		using _match_data_type = _pcre2_data<utf>::match_data_type;
		using _string_type = _pcre2_data<utf>::string_type;
		using _string_char_type = _pcre2_data<utf>::string_char_type;
		using _match_result_type = basic_match_result<utf>;
		using _string_ptr_type = _pcre2_data<utf>::string_ptr_type;

		size_t* _copies_num;
		_code_type* _code;
		_match_data_type* _match_data;

	public:
		constexpr basic_regex(const _string_char_type* pattern, size_t pattern_size,
			regex_compile_options opts = regex_compile_options::NONE) {
			_copies_num = new size_t(1);

			int error_code = 0;
			size_t error_offset = 0;
			_code = _pcre2_data<utf>::compile((_string_ptr_type)pattern,
				pattern_size, (uint32_t)opts, &error_code, &error_offset, nullptr);

			if (_code == nullptr) {
				throw regex_exception(error_code, error_offset);
			}

			_match_data = _pcre2_data<utf>::match_data_from_pattern(_code, nullptr);
		}
		constexpr basic_regex(const _string_type& pattern, 
			regex_compile_options opts = regex_compile_options::NONE) 
			: basic_regex(pattern.c_str(), pattern.length(), opts) {}
		template<size_t N>
		constexpr basic_regex(const _string_char_type (&pattern)[N], 
			regex_compile_options opts = regex_compile_options::NONE) 
			: basic_regex(pattern, N - 1, opts) {}
		constexpr basic_regex(const basic_regex<utf>& r) 
			: _code(r._code), _match_data(r._match_data), _copies_num(r._copies_num) {	
			++(*_copies_num);
		}
		virtual ~basic_regex() {
			--(*_copies_num);
			if (*_copies_num == 0) {
				_pcre2_data<utf>::match_data_free(_match_data);
				_match_data = nullptr;

				_pcre2_data<utf>::code_free(_code);
				_code = nullptr;
			}
		}

		constexpr basic_regex<utf>& operator=(const basic_regex<utf>& r) {
			this->_code = r._code;
			this->_match_data = r._match_data;
			this->_copies_num = r._copies_num;
			++(*_copies_num);
			return *this;
		}

#pragma region MATCH_WITH_RESULT
		constexpr bool match(const _string_char_type* text, size_t text_size,
			_match_result_type& result, regex_match_options opts, size_t offset = 0) const {
			int match_code =
				_pcre2_data<utf>::match(_code, (_string_ptr_type)text,
					text_size, offset, (uint32_t)opts, _match_data, nullptr);

			if (match_code > 0 && match_code != PCRE2_ERROR_NOMATCH) {
				size_t* ovector = _pcre2_data<utf>::get_ovector_ptr(_match_data);
				result = match_result(ovector[0] - offset,
					_string_type(text + ovector[0], ovector[1] - ovector[0]));
				return true;
			}

			result = match_result(match_code);
			return false;
		}
		constexpr bool match(const _string_type& text, _match_result_type& result, 
			regex_match_options opts, size_t offset = 0) const {
			return match(text.c_str(), text.length(), result, opts, offset);
		}
		template<size_t N>
		constexpr bool match(const _string_char_type (&text)[N], _match_result_type& result,
			regex_match_options opts, size_t offset = 0) const {
			return match(text, N - 1, result, opts, offset);
		}

		constexpr bool match(const _string_char_type* text, size_t text_size,
			_match_result_type& result, size_t offset = 0) const {
			return match(text, text_size, result, regex_match_options::NONE, offset);
		}
		constexpr bool match(const _string_type& text, _match_result_type& result, size_t offset = 0) const {
			return match(text.c_str(), text.length(), result, offset);
		}
		template<size_t N>
		constexpr bool match(const _string_char_type(&text)[N], _match_result_type& result, size_t offset = 0) const {
			return match(text, N - 1, result, offset);
		}
#pragma endregion

#pragma region MATCH
		constexpr bool match(const _string_char_type* text, size_t text_size,
			regex_match_options opts, size_t offset = 0) const {
			int match_code =
				_pcre2_data<utf>::match(_code, (_string_ptr_type)text,
					text_size, offset, (uint32_t)opts, _match_data, nullptr);

			return match_code > 0 && match_code != PCRE2_ERROR_NOMATCH;
		}
		constexpr bool match(const _string_type& text, regex_match_options opts, size_t offset = 0) const {
			return match(text.c_str(), text.length(), opts, offset);
		}
		template<size_t N>
		constexpr bool match(const _string_char_type(&text)[N], regex_match_options opts, size_t offset = 0) const {
			return match(text, N - 1, opts, offset);
		}

		constexpr bool match(const _string_char_type* text, size_t text_size, size_t offset = 0) const {
			return match(text, text_size, regex_match_options::NONE, offset);
		}
		constexpr bool match(const _string_type& text, size_t offset = 0) const {
			return match(text.c_str(), text.length(), offset);
		}
		template<size_t N>
		constexpr bool match(const _string_char_type(&text)[N], size_t offset = 0) const {
			return match(text, N - 1, offset);
		}

#pragma endregion

#pragma region MATCH_AT_WITH_RESULT
		constexpr bool match_at(const _string_char_type* text, size_t text_size,
			_match_result_type& result, size_t offset = 0) const {
			if (!match(text, text_size, result, offset))
				return false;

			if (result.getOffset() != 0) {
				result = match_result(0, "");
				return false;
			}

			return true;
		}
		constexpr bool match_at(const _string_type& text, _match_result_type& result, size_t offset = 0) const {
			return match_at(text.c_str(), text.length(), result, offset);
		}
		template<size_t N>
		constexpr bool match_at(const _string_char_type(&text)[N], _match_result_type& result, size_t offset = 0) const {
			return match_at(text, N - 1, result, offset);
		}
#pragma endregion

#pragma region MATCH_AT
		constexpr bool match_at(const _string_char_type* text, size_t text_size, size_t offset = 0) const {
			_match_result_type result;
			return match_at(text, text_size, result, offset);
		}
		constexpr bool match_at(const _string_type& text, size_t offset = 0) const {
			return match_at(text.c_str(), text.length(), offset);
		}
		template<size_t N>
		constexpr bool match_at(const _string_char_type(&text)[N], size_t offset = 0) const {
			return match_at(text, N - 1, offset);
		}
#pragma endregion
	};
}