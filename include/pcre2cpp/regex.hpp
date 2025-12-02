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
#include "regex_compile_options.hpp"
#include "regex_match_options.hpp"
#include "pcre2cpp_exceptions.hpp"

namespace pcre2cpp {
	template<size_t utf>
	class basic_regex {
	private:
		using _code_type = _pcre2_data<utf>::code_type;
		using _code_ptr = std::shared_ptr<_code_type>;
		using _match_data_type = _pcre2_data<utf>::match_data_type;
		using _match_data_ptr = std::shared_ptr<_match_data_type>;
		using _string_type = _pcre2_data<utf>::string_type;
		using _string_char_type = _pcre2_data<utf>::string_char_type;
		using _match_result_type = basic_match_result<utf>;
		using _string_ptr_type = _pcre2_data<utf>::string_ptr_type;
		using _named_sub_values_table = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;
#ifndef PCRE2CPP_NO_EXCEPTIONS
		using _regex_exception = basic_regex_exception<utf>;
#endif

		_code_ptr _code = nullptr;
		_match_data_ptr _match_data = nullptr;
		_named_sub_values_table_ptr _named_sub_values = nullptr;

#ifdef PCRE2CPP_NO_EXCEPTIONS
		int _error_code = 0;
		size_t _error_offset = 0;
		bool _initialized = false;
#endif

	public:
		constexpr explicit basic_regex(const _string_char_type* pattern, size_t pattern_size,
			regex_compile_options opts = static_cast<regex_compile_options>(regex_compile_options_bits::NONE)) PCRE2CPP_NOEXCEPT {

			// Compile Code
#ifdef PCRE2CPP_NO_EXCEPTIONS
			_code_type* code = _pcre2_data<utf>::compile(reinterpret_cast<_string_ptr_type>(pattern),
				pattern_size, opts, &_error_code, &_error_offset, nullptr);
#else
			int error_code = 0;
			size_t error_offset = 0;
			_code_type* code = _pcre2_data<utf>::compile(reinterpret_cast<_string_ptr_type>(pattern),
				pattern_size, opts, &error_code, &error_offset, nullptr);
#endif

			if (code == nullptr) {
#ifdef PCRE2CPP_NO_EXCEPTIONS
				return;
#else
				throw _regex_exception(error_code, error_offset);
#endif
			}

			_code = std::shared_ptr<_code_type>(code, _pcre2_data<utf>::code_free);

			// Get Named Sub Values
			_named_sub_values = std::make_shared<_named_sub_values_table>();

			size_t name_count = 0;
			unsigned char* name_table = nullptr;
			size_t name_entry_size = 0;

			_pcre2_data<utf>::get_info(_code.get(), PCRE2_INFO_NAMECOUNT, &name_count);
			_pcre2_data<utf>::get_info(_code.get(), PCRE2_INFO_NAMETABLE, &name_table);
			_pcre2_data<utf>::get_info(_code.get(), PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

			for (size_t i = 0; i != name_count; ++i) {
				unsigned char* entry = name_table + i * name_entry_size + 2;
				int index = _pcre2_data<utf>::substring_number_from_name(_code.get(), entry);

				unsigned char* entry_end = entry + 1;
				while (*entry_end != '\0' && entry_end - entry < name_entry_size - 3) {
					entry_end += 1;
				}
				(*_named_sub_values)[_string_type(entry, entry_end)] = static_cast<size_t>(index) - 1;
			}

			// Create Match Data
			_match_data_type* match_data = _pcre2_data<utf>::match_data_from_pattern(_code.get(), nullptr);
			_match_data = std::shared_ptr<_match_data_type>(match_data, _pcre2_data<utf>::match_data_free);

#ifdef PCRE2CPP_NO_EXCEPTIONS
			_initialized = true;
#endif
		}
		constexpr explicit basic_regex(const _string_type& pattern, 
			regex_compile_options opts = static_cast<regex_compile_options>(regex_compile_options_bits::NONE)) PCRE2CPP_NOEXCEPT
			: basic_regex(pattern.c_str(), pattern.length(), opts) {}
		template<size_t N>
		constexpr explicit basic_regex(const _string_char_type(&pattern)[N],
			regex_compile_options opts = static_cast<regex_compile_options>(regex_compile_options_bits::NONE)) PCRE2CPP_NOEXCEPT
			: basic_regex(pattern, N - 1, opts) {}

		constexpr basic_regex(const basic_regex<utf>& r) noexcept = default;
		
		virtual ~basic_regex() noexcept = default;

		constexpr basic_regex<utf>& operator=(const basic_regex<utf>& r) noexcept = default;

#ifdef PCRE2CPP_NO_EXCEPTIONS
#pragma region CHECK_INITIALIZATION
		constexpr bool is_initialized() const noexcept {
			return _initialized;
		}
#pragma endregion CHECK_INITIALIZATION

#pragma region ERROR
		constexpr _string_type get_error_message() const noexcept {
			if (_initialized) {
				if constexpr (utf == 8) {
					return "";
				}
				else if constexpr (utf == 16) {
					return L"";
				}
				else if constexpr (utf == 32) {
					return U"";
				}
			}
			return pcre2cpp::generate_error_message<utf>(_error_code, _error_offset);
		}
#pragma endregion ERROR
#endif

#pragma region MATCH
		constexpr bool match(const _string_char_type* text, size_t text_size, size_t offset = 0,
			regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
#ifdef PCRE2CPP_NO_EXCEPTION
			if (_code == nullptr || _match_data == nullptr)
				return false;
#endif

			int match_code =
				_pcre2_data<utf>::match(_code.get(), reinterpret_cast<_string_ptr_type>(text),
					text_size, offset, opts, _match_data.get(), nullptr);

			return match_code > 0 && match_code != PCRE2_ERROR_NOMATCH;
		}
		constexpr bool match(const _string_type& text, size_t offset = 0, 
			regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
			return match(text.c_str(), text.length(), offset, opts);
		}
#pragma endregion MATCH

#pragma region MATCH_WITH_RESULT
		constexpr bool match(const _string_char_type* text, size_t text_size,
			_match_result_type& result, size_t offset = 0, 
			regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {

#ifdef PCRE2CPP_NO_EXCEPTION
			if (_code == nullptr || _match_data == nullptr || _named_sub_values == nullptr)
				return false;
#endif

			int match_code =
				_pcre2_data<utf>::match(_code.get(), reinterpret_cast<_string_ptr_type>(text),
					text_size, offset, opts, _match_data.get(), nullptr);

			if (match_code > 0 && match_code != PCRE2_ERROR_NOMATCH) {
				size_t* ovector = _pcre2_data<utf>::get_ovector_ptr(_match_data.get());
				std::pair<size_t, _string_type> value = { ovector[0] - offset,
					_string_type(text + ovector[0], ovector[1] - ovector[0]) };

				size_t ovectors_count = _pcre2_data<utf>::get_ovector_count(_match_data.get());
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
			size_t offset = 0, regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
			return match(text.c_str(), text.length(), result, offset, opts);
		}
#pragma endregion MATCH_WITH_RESULT

#pragma region MATCH_AT
		constexpr bool match_at(const _string_char_type* text, size_t text_size, size_t offset = 0) const noexcept {
			_match_result_type result;
			return match_at(text, text_size, result, offset);
		}
		constexpr bool match_at(const _string_type& text, size_t offset = 0) const noexcept {
			return match_at(text.c_str(), text.length(), offset);
		}
#pragma endregion MATCH_AT

#pragma region MATCH_AT_WITH_RESULT
		constexpr bool match_at(const _string_char_type* text, size_t text_size,
			_match_result_type& result, size_t offset = 0) const noexcept {
			if (!match(text, text_size, result, offset))
				return false;

			if (result.get_result_relative_offset() != 0) {
				result = _match_result_type(offset, _named_sub_values);
				return false;
			}

			return true;
		}
		constexpr bool match_at(const _string_type& text, _match_result_type& result, size_t offset = 0) const noexcept {
			return match_at(text.c_str(), text.length(), result, offset);
		}
#pragma endregion MATCH_AT_WITH_RESULT

#pragma region MATCH_ALL_WITH_RESULT
		constexpr bool match_all(const _string_char_type* text, size_t text_size, 
			std::vector<_match_result_type>& results, size_t offset = 0) const noexcept {
			std::vector<_match_result_type> temp_results;

			size_t start_offset = offset;
			_match_result_type result;
			while (match(text + offset, text_size - offset, result)) {
				temp_results.push_back(_match_result_type(start_offset, std::make_pair(offset + result.get_result_relative_offset(), result.get_result_value()), 
					result.get_sub_results(), _named_sub_values));
				offset += result.get_result_relative_offset() + result.get_result_value().length();
			}

			results = temp_results;
			temp_results.clear();
			return results.size() != 0;
		}
		constexpr bool match_all(const _string_type& text, std::vector<_match_result_type>& results, size_t offset = 0) const noexcept {
			return match_all(text.c_str(), text.length(), results, offset);
		}

		constexpr bool match_all(const _string_char_type* text, size_t text_size, _match_result_type*& results, 
			size_t& results_count, size_t offset = 0) const noexcept {
			std::vector<_match_result_type> temp_results;
			bool result = match_all(text, text_size, temp_results, offset);

			results = new _match_result_type[temp_results.size()];
			std::copy(temp_results.begin(), temp_results.end(), results);
			results_count = temp_results.size();

			temp_results.clear();

			return result;
		}
		constexpr bool match_all(const _string_type& text, _match_result_type*& results, 
			size_t& results_count, size_t offset = 0) const noexcept {
			return match_all(text.c_str(), text.length(), results, results_count, offset);
		}
#pragma endregion MATCH_ALL_WITH_RESULT
	};
}