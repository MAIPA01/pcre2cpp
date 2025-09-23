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
		std::unordered_map<_string_type, size_t> _named_sub_values{};

	public:
		constexpr basic_regex(const _string_char_type* pattern, size_t pattern_size,
			regex_compile_options opts = regex_compile_options::NONE) {
			_copies_num = new size_t(1);

			int error_code = 0;
			size_t error_offset = 0;
			_code = _pcre2_data<utf>::compile((_string_ptr_type)pattern,
				pattern_size, (uint32_t)opts, &error_code, &error_offset, nullptr);

			if (_code == nullptr) {
				throw basic_regex_exception<utf>(error_code, error_offset);
			}

			size_t name_count = 0;
			unsigned char* name_table = nullptr;
			size_t name_entry_size = 0;

			_pcre2_data<utf>::get_info(_code, PCRE2_INFO_NAMECOUNT, &name_count);
			_pcre2_data<utf>::get_info(_code, PCRE2_INFO_NAMETABLE, &name_table);
			_pcre2_data<utf>::get_info(_code, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

			for (size_t i = 0; i != name_count; ++i) {
				unsigned char* entry = name_table + i * name_entry_size + 2;
				int index = _pcre2_data<utf>::substring_number_from_name(_code, entry);

				unsigned char* entry_end = entry + 1;
				while (*entry_end != '\0' && entry_end - entry < name_entry_size - 3) {
					entry_end += 1;
				}
				_named_sub_values[_string_type(entry, entry_end)] = ((size_t)index) - 1;
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
			: _code(r._code), _match_data(r._match_data), _named_sub_values(r._named_sub_values),
			_copies_num(r._copies_num) {
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
			this->_named_sub_values = r._named_sub_values;
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
				std::pair<size_t, _string_type> value = { ovector[0] - offset,
					_string_type(text + ovector[0], ovector[1] - ovector[0]) };

				size_t ovectors_count = _pcre2_data<utf>::get_ovector_count(_match_data);
				std::vector<std::pair<size_t, _string_type>> sub_values;
				for (size_t i = 1; i != ovectors_count; ++i) {
					sub_values.push_back({
						ovector[i * 2] - ovector[0],
						_string_type(text + ovector[i * 2], ovector[i * 2 + 1] - ovector[i * 2])
						});
				}

				result = _match_result_type(offset, value, sub_values, _named_sub_values);
				return true;
			}

			result = _match_result_type(match_code);
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
#pragma endregion MATCH_WITH_RESULT

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

#pragma endregion MATCH

#pragma region MATCH_AT_WITH_RESULT
		constexpr bool match_at(const _string_char_type* text, size_t text_size,
			_match_result_type& result, size_t offset = 0) const {
			if (!match(text, text_size, result, offset))
				return false;

			if (result.get_result_relative_offset() != 0) {
				result = _match_result_type(offset, _named_sub_values);
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
#pragma endregion MATCH_AT_WITH_RESULT

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
#pragma endregion MATCH_AT

#pragma region MATCH_ALL_WITH_RESULT
		constexpr bool match_all(const _string_char_type* text, size_t text_size, std::vector<_match_result_type>& results, size_t offset = 0) const {
			std::vector<_match_result_type> temp_results;

			size_t start_offset = offset;
			_match_result_type result;
			while (match(text + offset, text_size - offset, result)) {
				temp_results.push_back(_match_result_type(start_offset, { offset + result.get_result_relative_offset(), result.get_result_value() }, 
					result.get_sub_results(), _named_sub_values));
				offset += result.get_result_relative_offset() + result.get_result_value().length();
			}

			results = temp_results;
			temp_results.clear();
			return results.size() != 0;
		}
		constexpr bool match_all(const _string_type& text, std::vector<_match_result_type>& results, size_t offset = 0) const {
			return match_all(text.c_str(), text.length(), results, offset);
		}
		template<size_t N>
		constexpr bool match_all(const _string_char_type(&text)[N], std::vector<_match_result_type>& results, size_t offset = 0) const {
			return match_all(text, N - 1, results, offset);
		}

		constexpr bool match_all(const _string_char_type* text, size_t text_size, _match_result_type*& results, size_t& results_count, size_t offset = 0) const {
			std::vector<_match_result_type> temp_results;
			bool result = match_all(text, text_size, temp_results, offset);

			results = new _match_result_type[temp_results.size()];
			std::copy(temp_results.begin(), temp_results.end(), results);
			results_count = temp_results.size();

			temp_results.clear();

			return result;
		}
		constexpr bool match_all(const _string_type& text, _match_result_type*& results, size_t& results_count, size_t offset = 0) const {
			return match_all(text.c_str(), text.length(), results, results_count, offset);
		}
		template<size_t N>
		constexpr bool match_all(const _string_char_type(&text)[N], _match_result_type*& results, size_t& results_count, size_t offset = 0) const {
			return match_all(text, N - 1, results, results_count, offset);
		}
#pragma endregion MATCH_ALL_WITH_RESULT
	};
}