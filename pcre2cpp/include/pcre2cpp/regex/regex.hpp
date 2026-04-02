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
#ifndef _PCRE2CPP_REGEX_HPP_
	#define _PCRE2CPP_REGEX_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/exceptions/exceptions.hpp>
		#include <pcre2cpp/regex/regex_compile_options.hpp>
		#include <pcre2cpp/regex/regex_match_options.hpp>
		#include <pcre2cpp/types.hpp>
		#include <pcre2cpp/utils/pcre2_data.hpp>

namespace pcre2cpp {
	template<size_t utf>
	class basic_regex {
	private:
		using _pcre2_data_t				  = pcre2_data<utf>;

		using _code_type				  = typename _pcre2_data_t::code_type;
		using _code_ptr					  = std::shared_ptr<_code_type>;
		using _match_data_type			  = typename _pcre2_data_t::match_data_type;
		using _match_data_ptr			  = std::shared_ptr<_match_data_type>;
		using _string_type				  = typename _pcre2_data_t::string_type;
		using _string_view_type			  = typename _pcre2_data_t::string_view_type;
		using _string_char_type			  = typename _pcre2_data_t::string_char_type;
		using _match_result_type		  = basic_match_result<utf>;
		using _sptr_type				  = typename _pcre2_data_t::sptr_type;
		using _named_sub_values_table	  = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;
		using _uchar_type				  = typename _pcre2_data_t::uchar_type;
		#if _PCRE2CPP_HAS_EXCEPTIONS
		using _regex_exception = basic_regex_exception<utf>;
		#endif

		_code_ptr _code								  = nullptr;
		_match_data_ptr _match_data					  = nullptr;
		_named_sub_values_table_ptr _named_sub_values = nullptr;

		#if !_PCRE2CPP_HAS_EXCEPTIONS
		int _error_code		 = 0;
		size_t _error_offset = 0;
		bool _initialized	 = false;
		#endif

	public:
		_PCRE2CPP_CONSTEXPR20 explicit basic_regex(
		  const _string_char_type* pattern, const size_t pattern_size,
		  const regex_compile_options opts = static_cast<regex_compile_options>(regex_compile_options_bits::NONE)
		) _PCRE2CPP_NOEXCEPT {
				// Compile Code
		#if !_PCRE2CPP_HAS_EXCEPTIONS
			_code_type* code = _pcre2_data_t::compile(reinterpret_cast<_sptr_type>(pattern), pattern_size, opts, &_error_code,
			  &_error_offset, nullptr);
		#else
			int error_code		= 0;
			size_t error_offset = 0;
			_code_type* code	= _pcre2_data_t::compile(reinterpret_cast<_sptr_type>(pattern), pattern_size, opts, &error_code,
			  &error_offset, nullptr);
		#endif

				if (code == nullptr) {
		#if !_PCRE2CPP_HAS_EXCEPTIONS
					std::string message = fmt::format("Failed to initialize code: {}", generate_error_message<8>(_error_code, _error_offset));
					pcre2cpp_assert(false, "{}", message);
					return;
		#else
					throw _regex_exception(error_code, error_offset);
		#endif
				}

			_code					= std::shared_ptr<_code_type>(code, _pcre2_data_t::code_free);

			// Get Named Sub Values
			_named_sub_values		= std::make_shared<_named_sub_values_table>();

			size_t name_count		= 0;
			_uchar_type* name_table = nullptr;
			size_t name_entry_size	= 0;

			_pcre2_data_t::get_info(_code.get(), PCRE2_INFO_NAMECOUNT, &name_count);
			_pcre2_data_t::get_info(_code.get(), PCRE2_INFO_NAMETABLE, &name_table);
			_pcre2_data_t::get_info(_code.get(), PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

				for (size_t i = 0; i != name_count; ++i) {
					_uchar_type* entry	   = name_table + i * name_entry_size + 2;
					const int index		   = _pcre2_data_t::substring_number_from_name(_code.get(), entry);

					_uchar_type* entry_end = entry + 1;
						while (*entry_end != 0 && entry_end - entry < name_entry_size - 3) { entry_end += 1; }
					_named_sub_values->emplace(_string_type(entry, entry_end), static_cast<size_t>(index) - 1);
				}

			// Create Match Data
			_match_data_type* match_data = _pcre2_data_t::match_data_from_pattern(_code.get(), nullptr);
			_match_data					 = std::shared_ptr<_match_data_type>(match_data, _pcre2_data_t::match_data_free);

		#if !_PCRE2CPP_HAS_EXCEPTIONS
			_initialized = true;
		#endif
		}

		_PCRE2CPP_CONSTEXPR17 explicit basic_regex(
		  const _string_view_type pattern,
		  const regex_compile_options opts = static_cast<regex_compile_options>(regex_compile_options_bits::NONE)
		) _PCRE2CPP_NOEXCEPT : basic_regex(pattern.data(), pattern.size(), opts) {}

		_PCRE2CPP_CONSTEXPR17 basic_regex(const basic_regex& other) noexcept			= default;
		_PCRE2CPP_CONSTEXPR17 basic_regex(basic_regex&& other) noexcept					= default;

		_PCRE2CPP_CONSTEXPR20 ~basic_regex() noexcept									= default;

		_PCRE2CPP_CONSTEXPR17 basic_regex& operator=(const basic_regex& other) noexcept = default;
		_PCRE2CPP_CONSTEXPR17 basic_regex& operator=(basic_regex&& other) noexcept		= default;

		#if !_PCRE2CPP_HAS_EXCEPTIONS
			#pragma region CHECK_INITIALIZATION

		_PCRE2CPP_CONSTEXPR17 bool is_initialized() const noexcept { return _initialized; }

			#pragma endregion CHECK_INITIALIZATION

			#pragma region ERROR

		_PCRE2CPP_CONSTEXPR17 _string_type get_error_message() const noexcept {
				if (_initialized) {
						if _PCRE2CPP_CONSTEXPR17 (utf == 8) { return ""; }
						else if _PCRE2CPP_CONSTEXPR17 (utf == 16) { return L""; }
						else if _PCRE2CPP_CONSTEXPR17 (utf == 32) { return U""; }
				}
			return pcre2cpp::generate_error_message<utf>(_error_code, _error_offset);
		}

			#pragma endregion ERROR
		#endif

		#pragma region MATCH

		_PCRE2CPP_CONSTEXPR17 bool match(const _string_char_type* text, const size_t text_size, const size_t offset = 0,
		  const regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
		#if !_PCRE2CPP_HAS_EXCEPTIONS
				if (_code == nullptr || _match_data == nullptr) {
					pcre2cpp_assert(false, "Regex was not initialized!!");
					return false;
				}
		#endif

			const int match_code = _pcre2_data_t::match(_code.get(), reinterpret_cast<_sptr_type>(text), text_size, offset, opts,
			  _match_data.get(), nullptr);

			return match_code != PCRE2_ERROR_NOMATCH && match_code > 0;
		}

		_PCRE2CPP_CONSTEXPR17 bool match(const _string_view_type text, const size_t offset = 0,
		  const regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
			return match(text.data(), text.size(), offset, opts);
		}

		#pragma endregion MATCH

		#pragma region MATCH_WITH_RESULT

		_PCRE2CPP_CONSTEXPR17 bool match(const _string_char_type* text, const size_t text_size, _match_result_type& result,
		  const size_t offset			 = 0,
		  const regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {

		#if !_PCRE2CPP_HAS_EXCEPTIONS
				if (_code == nullptr || _match_data == nullptr || _named_sub_values == nullptr) {
					pcre2cpp_assert(false, "Regex was not initialized!!");
					return false;
				}
		#endif

			const int match_code = _pcre2_data_t::match(_code.get(), reinterpret_cast<_sptr_type>(text), text_size, offset, opts,
			  _match_data.get(), nullptr);

				if (match_code != PCRE2_ERROR_NOMATCH && match_code > 0) {
					const size_t* ovector = _pcre2_data_t::get_ovector_ptr(_match_data.get());
					std::pair<size_t, _string_type> value =
					  std::make_pair(ovector[0] - offset, _string_type(text + ovector[0], ovector[1] - ovector[0]));

					const size_t ovectors_count = _pcre2_data_t::get_ovector_count(_match_data.get());
					std::vector<std::pair<size_t, _string_type> > sub_values;
						for (size_t i = 1; i != ovectors_count; ++i) {
							sub_values.emplace_back(ovector[i * 2] - ovector[0],
							  _string_type(text + ovector[i * 2], ovector[i * 2 + 1] - ovector[i * 2]));
						}

					result = _match_result_type(offset, value, sub_values, _named_sub_values);
					return true;
				}

			result = _match_result_type(match_code);
			return false;
		}

		_PCRE2CPP_CONSTEXPR17 bool match(const _string_view_type text, _match_result_type& result, const size_t offset = 0,
		  const regex_match_options opts = static_cast<regex_match_options>(regex_match_options_bits::NONE)) const noexcept {
			return match(text.data(), text.size(), result, offset, opts);
		}

		#pragma endregion MATCH_WITH_RESULT

		#pragma region MATCH_AT

		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_char_type* text, const size_t text_size,
		  const size_t offset = 0) const noexcept {
			_match_result_type result;
			return match_at(text, text_size, result, offset);
		}

		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_view_type text, const size_t offset = 0) const noexcept {
			return match_at(text.data(), text.size(), offset);
		}

		#pragma endregion MATCH_AT

		#pragma region MATCH_AT_WITH_RESULT

		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_char_type* text, const size_t text_size, _match_result_type& result,
		  const size_t offset = 0) const noexcept {
				if (!match(text, text_size, result, offset)) { return false; }

				if (result.get_result_relative_offset() != 0) {
					result = _match_result_type(offset, _named_sub_values);
					return false;
				}

			return true;
		}

		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_view_type text, _match_result_type& result,
		  const size_t offset = 0) const noexcept {
			return match_at(text.data(), text.size(), result, offset);
		}

		#pragma endregion MATCH_AT_WITH_RESULT

		#pragma region MATCH_ALL_WITH_RESULT

		_PCRE2CPP_CONSTEXPR17 bool match_all(const _string_char_type* text, const size_t text_size,
		  std::vector<_match_result_type>& results, size_t offset = 0) const noexcept {
			std::vector<_match_result_type> temp_results;

			size_t start_offset = offset;
			_match_result_type result;
				while (match(text + offset, text_size - offset, result)) {
					temp_results.emplace_back(start_offset,
					  std::make_pair(offset + result.get_result_relative_offset(), result.get_result_value()),
					  result.get_sub_results(), _named_sub_values);
					offset += result.get_result_relative_offset() + result.get_result_value().length();
				}

			results = temp_results;
			temp_results.clear();
			return results.size() != 0;
		}

		_PCRE2CPP_CONSTEXPR17 bool match_all(const _string_view_type text, std::vector<_match_result_type>& results,
		  const size_t offset = 0) const noexcept {
			return match_all(text.data(), text.size(), results, offset);
		}

		_PCRE2CPP_CONSTEXPR17 bool match_all(const _string_char_type* text, const size_t text_size, _match_result_type*& results,
		  size_t& results_count, const size_t offset = 0) const noexcept {
			std::vector<_match_result_type> temp_results;
			const bool result = match_all(text, text_size, temp_results, offset);

			results			  = new _match_result_type[temp_results.size()];
			std::copy(temp_results.begin(), temp_results.end(), results);
			results_count = temp_results.size();

			temp_results.clear();

			return result;
		}

		_PCRE2CPP_CONSTEXPR17 bool match_all(const _string_view_type text, _match_result_type*& results, size_t& results_count,
		  const size_t offset = 0) const noexcept {
			return match_all(text.data(), text.size(), results, results_count, offset);
		}

		#pragma endregion MATCH_ALL_WITH_RESULT
	};
} // namespace pcre2cpp
	#endif
#endif