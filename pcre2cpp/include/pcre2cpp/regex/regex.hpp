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
		#include <pcre2cpp/match/match_error_codes.hpp>
		#include <pcre2cpp/match/match_options.hpp>
		#include <pcre2cpp/match/match_result.hpp>
		#include <pcre2cpp/regex/compile_options.hpp>
		#include <pcre2cpp/types.hpp>
		#include <pcre2cpp/utils/pcre2_data.hpp>

namespace pcre2cpp {
	/**
	 * @brief Basic PCRE2 Regex container
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	class basic_regex {
	private:
		using _pcre2_data_t				  = utils::pcre2_data<utf>;

		using _code_type				  = typename _pcre2_data_t::code_type;
		using _code_ptr					  = std::shared_ptr<_code_type>;
		using _match_data_type			  = typename _pcre2_data_t::match_data_type;
		using _match_data_ptr			  = std::shared_ptr<_match_data_type>;
		using _string_type				  = typename _pcre2_data_t::string_type;
		using _string_view_type			  = typename _pcre2_data_t::string_view_type;
		using _string_char_type			  = typename _pcre2_data_t::string_char_type;
		using _match_value_type			  = basic_match_value<utf>;
		using _match_result_type		  = basic_match_result<utf>;
		using _sptr_type				  = typename _pcre2_data_t::sptr_type;
		using _named_sub_values_table	  = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;
		using _uchar_type				  = typename _pcre2_data_t::uchar_type;
		#if _PCRE2CPP_HAS_EXCEPTIONS
		using _regex_exception = basic_regex_exception<utf>;
		#endif

		/// @brief pointer to compiled pcre2 code
		_code_ptr _code								  = nullptr;
		/// @brief pointer to match data of pcre2 code
		_match_data_ptr _match_data					  = nullptr;
		/// @brief pointer to conversion table of named groups to their index
		_named_sub_values_table_ptr _named_sub_values = nullptr;

		/// @brief error code
		int _error_code								  = 0;
		/// @brief error offset
		size_t _error_offset						  = 0;

		static _PCRE2CPP_CONSTEXPR17 _string_type _get_regex_not_initialized_error() noexcept {
				if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) { return "Regex was not initialized!!"; }
				else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) { return u"Regex was not initialized!!"; }
				else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) { return U"Regex was not initialized!!"; }
				else { return _string_type(); }
		}

	public:
		/// @brief basic regex container with pattern and compile options
		_PCRE2CPP_CONSTEXPR20 explicit basic_regex(const _string_view_type pattern,
		  const compile_options opts = compile_options_bits::None) _PCRE2CPP_NOEXCEPT {
			// Compile Code
			_code_type* code = _pcre2_data_t::compile(reinterpret_cast<_sptr_type>(pattern.data()), pattern.size(), opts,
			  &_error_code, &_error_offset, nullptr);

				if (code == nullptr) {
		#if !_PCRE2CPP_HAS_EXCEPTIONS
					std::string message = fmt::format("Failed to initialize code: {}",
					  generate_error_message<utf_type::UTF_8>(_error_code, _error_offset));
					pcre2cpp_assert(false, "{}", message);
					return;
		#else
					throw _regex_exception(_error_code, _error_offset);
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
		}

		/// @brief default copy constructor
		_PCRE2CPP_CONSTEXPR17 basic_regex(const basic_regex& other) noexcept			= default;
		/// @brief default move constructor
		_PCRE2CPP_CONSTEXPR17 basic_regex(basic_regex&& other) noexcept					= default;

		/// @brief default destructor
		_PCRE2CPP_CONSTEXPR20 ~basic_regex() noexcept									= default;

		/// @brief default copy assign operator
		_PCRE2CPP_CONSTEXPR17 basic_regex& operator=(const basic_regex& other) noexcept = default;
		/// @brief default move assign operator
		_PCRE2CPP_CONSTEXPR17 basic_regex& operator=(basic_regex&& other) noexcept		= default;

		#pragma region CHECK_INITIALIZATION

		/// @brief returns true if regex was initialized
		_PCRE2CPP_CONSTEXPR17 bool is_initialized() const noexcept { return _code != nullptr; }

		#pragma endregion CHECK_INITIALIZATION

		#pragma region ERROR

		/// @brief returns error message if there is any compilation error
		_PCRE2CPP_CONSTEXPR17 _string_type get_error_message() const noexcept {
				if (is_initialized()) {
						if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) { return ""; }
						else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) { return L""; }
						else if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) { return U""; }
						else { return _string_type(); }
				}
			return pcre2cpp::generate_error_message<utf>(_error_code, _error_offset);
		}

		#pragma endregion ERROR

		/// @brief returns true if match was found
		_PCRE2CPP_CONSTEXPR17 bool match(const _string_view_type text, const size_t offset = 0,
		  const match_options opts = match_options_bits::None) const _PCRE2CPP_NOEXCEPT {
				if (!is_initialized()) {
		#if !_PCRE2CPP_HAS_EXCEPTIONS
					pcre2cpp_assert(false, "Regex was not initialized!!");
					return false;
		#else
					throw _regex_exception(_get_regex_not_initialized_error());
		#endif
				}

			const int match_code = _pcre2_data_t::match(_code.get(), reinterpret_cast<_sptr_type>(text.data()), text.size(),
			  offset, opts, _match_data.get(), nullptr);

			return match_code != static_cast<int>(match_error_codes::NoMatch) && match_code > 0;
		}

		/// @brief returns true if match was found and result is stored in result variable
		_PCRE2CPP_CONSTEXPR20 bool match(const _string_view_type text, _match_result_type& result, const size_t offset = 0,
		  const match_options opts = match_options_bits::None) const noexcept {
				if (!is_initialized()) {
		#if !_PCRE2CPP_HAS_EXCEPTIONS
					pcre2cpp_assert(false, "Regex was not initialized!!");
					return false;
		#else
					throw _regex_exception(_get_regex_not_initialized_error());
		#endif
				}

			const int match_code = _pcre2_data_t::match(_code.get(), reinterpret_cast<_sptr_type>(text.data()), text.size(),
			  offset, opts, _match_data.get(), nullptr);

				if (match_code == static_cast<int>(match_error_codes::NoMatch) || match_code <= 0) {
					result = _match_result_type(static_cast<match_error_codes>(match_code));
					return false;
				}

			const size_t* offsetVector		= _pcre2_data_t::get_ovector_ptr(_match_data.get());
			const size_t matchStart			= offsetVector[0];
			const size_t matchEnd			= offsetVector[1];
			_match_value_type value			= { .relative_offset = matchStart - offset,
				.value									 = _string_type(text.substr(matchStart, matchEnd - matchStart)) };

			const size_t offsetVectorsCount = _pcre2_data_t::get_ovector_count(_match_data.get());
			std::vector<std::optional<sub_match_value> > sub_values;
			sub_values.reserve(offsetVectorsCount);
				for (size_t i = 1; i != offsetVectorsCount; ++i) {
					const size_t subMatchStart = offsetVector[i * 2];
					const size_t subMatchEnd   = offsetVector[i * 2 + 1];

						if (subMatchStart == PCRE2_UNSET || subMatchEnd == PCRE2_UNSET) { sub_values.emplace_back(); }
						else {
							sub_values.push_back(sub_match_value { .relative_offset = subMatchStart - matchStart,
								.size												= subMatchEnd - subMatchStart });
						}
				}

			result = _match_result_type(offset, value, sub_values, _named_sub_values);
			return true;
		}

		/// @brief returns true if match was found, and it has relative offset == 0
		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_view_type text, const size_t offset = 0) const noexcept {
			_match_result_type result;
			return match_at(text, result, offset);
		}

		/// @brief returns true if match was found, and it has relative offset == 0 and result is stored in result variable
		_PCRE2CPP_CONSTEXPR17 bool match_at(const _string_view_type text, _match_result_type& result,
		  const size_t offset = 0) const noexcept {
				if (!match(text, result, offset)) { return false; }

				if (result.get_result_relative_offset() != 0) {
					result = _match_result_type(offset, _named_sub_values);
					return false;
				}

			return true;
		}

		/// @brief returns true if any match was found and all results store in results array
		_PCRE2CPP_CONSTEXPR17 bool match_all(const _string_view_type text, std::vector<_match_result_type>& results,
		  size_t offset = 0) const noexcept {
			size_t start_offset = offset;
			_match_result_type result;
				while (match(text, result, offset)) {
					results.emplace_back(start_offset,
					  _match_value_type { .relative_offset = offset - start_offset + result.get_result_relative_offset(),
						  .value						   = result.get_result_value() },
					  result.get_sub_results(), _named_sub_values);
					offset += result.get_result_relative_offset() + result.get_result_size();
				}

			return results.size() != 0;
		}
	};

	using u8regex  = basic_regex<utf_type::UTF_8>;
	using u16regex = basic_regex<utf_type::UTF_16>;
	using u32regex = basic_regex<utf_type::UTF_32>;
	using regex	   = u8regex;
} // namespace pcre2cpp
	#endif
#endif