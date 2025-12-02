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
#include "pcre2cpp_exceptions.hpp"

namespace pcre2cpp {
	template<size_t utf>
	class basic_match_result {
	public:
		static constexpr size_t bad_offset = static_cast<size_t>(-1);

	private:
		using _string_type = _pcre2_data<utf>::string_type;
		using _match_value = std::pair<size_t, _string_type>;
#ifndef PCRE2CPP_NO_EXCEPTIONS
		using _match_result_exception = basic_match_result_exception<utf>;
#endif
		using _named_sub_values_table = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;

		struct _value_result_data {
			size_t search_offset = bad_offset;
			_match_value result = { bad_offset, _string_type() };
			std::vector<_match_value> sub_results = {};
			_named_sub_values_table_ptr named_sub_values = {};
			bool found = false;
		};

		std::variant<int, _value_result_data> _data = _value_result_data();

		constexpr size_t _get_named_sub_result_idx(const _string_type& name) const PCRE2CPP_NOEXCEPT {
			auto& named_sub_values = std::get<_value_result_data>(_data).named_sub_values;
			auto itr = named_sub_values->find(name);

			if (itr == named_sub_values->end()) {
#ifdef PCRE2CPP_NO_EXCEPTIONS
				return static_cast<size_t>(-1);
#else
				if constexpr (utf == 8) {
					_string_type message = "Subexpression with provided name '";
					message += name;
					message += "' not found";
					throw _match_result_exception(message);
				}
				else if constexpr (utf == 16) {
					_string_type message = L"Subexpression with provided name '";
					message += name;
					message += L"' not found";
					throw _match_result_exception(message);
				}
				else if constexpr (utf == 32) {
					_string_type message = U"Subexpression with provided name '";
					message += name;
					message += U"' not found";
					throw _match_result_exception(message);
				}
#endif
			}

			return itr->second;
		}
		constexpr _match_value _get_sub_result(const size_t& idx) const PCRE2CPP_NOEXCEPT {
			auto& sub_results = std::get<_value_result_data>(_data).sub_results;
			if (idx >= sub_results.size()) {
#ifdef PCRE2CPP_NO_EXCEPTIONS
				if constexpr (utf == 8) {
					return { static_cast<size_t>(-1), "" };
				}
				else if constexpr (utf == 16) {
					return { static_cast<size_t>(-1), L"" };
				}
				else if constexpr (utf == 32) {
					return { static_cast<size_t>(-1), U"" };
				}
#else
				if constexpr (utf == 8) {
					throw basic_match_result_exception<utf>("Subexpression index out of bounds");
				}
				else if constexpr (utf == 16) {
					throw basic_match_result_exception<utf>(L"Subexpression index out of bounds");
				}
				else if constexpr (utf == 32) {
					throw basic_match_result_exception<utf>(U"Subexpression index out of bounds");
				}
#endif
			}
			return sub_results[idx];
		}

	public:
		
		constexpr basic_match_result() noexcept = default;
		constexpr basic_match_result(int error_code) noexcept
			: _data(error_code) {}
		constexpr basic_match_result(const size_t& search_offset, 
			const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data{ search_offset, { bad_offset, _string_type() }, {}, named_sub_values, false }) {}
		constexpr basic_match_result(const size_t& search_offset, const _match_value& result, 
			const std::vector<_match_value>& sub_results, 
			const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data{ search_offset, result, sub_results, named_sub_values, true }) { }
		constexpr basic_match_result(const basic_match_result<utf>& mr) noexcept = default;
		~basic_match_result() = default;

		constexpr basic_match_result<utf>& operator=(const basic_match_result<utf>& mr) noexcept = default;

#pragma region ERRORS
		constexpr bool has_error() const noexcept { return std::holds_alternative<int>(_data); }
		constexpr int get_error_code() const noexcept { 
			if (!has_error()) return 0;
			return std::get<int>(_data);
		}
#ifdef PCRE2CPP_NO_EXCEPTIONS
		constexpr _string_type get_error_message() const noexcept {
			if (!has_error()) {
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
			return pcre2cpp::generate_error_message<utf>(std::get<int>(_data));
		}
#else
		constexpr void throw_error() const {
			if (!has_error()) return;
			throw _match_result_exception(std::get<int>(_data));
		}
#endif
#pragma endregion ERRORS

#pragma region RESULTS
		constexpr bool has_result() const noexcept { 
			return std::holds_alternative<_value_result_data>(_data);
		}
		constexpr bool has_value() const noexcept {
			if (!has_result()) return false;
			return std::get<_value_result_data>(_data).found;
		}
		constexpr size_t get_search_offset() const noexcept {
			if (!has_result()) return bad_offset;
			return std::get<_value_result_data>(_data).search_offset;
		}

#pragma region RESULT
#pragma region OFFSETS
		constexpr size_t get_result_global_offset() const noexcept {
			if (!has_value()) return bad_offset;
			auto& value = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.first;
		}
		constexpr size_t get_result_relative_offset() const noexcept {
			if (!has_value()) return bad_offset;
			return std::get<_value_result_data>(_data).result.first;
		}
#pragma endregion OFFSETS

#pragma region VALUE
		constexpr _string_type get_result_value() const noexcept {
			if (!has_value()) return _string_type();
			return std::get<_value_result_data>(_data).result.second;
		}
#pragma endregion VALUE
#pragma endregion RESULT

#pragma region SUB_RESULTS
		constexpr std::vector<_match_value> get_sub_results() const noexcept {
			if (!has_value()) return {};
			return std::get<_value_result_data>(_data).sub_results;
		}

#pragma region OFFSETS
		constexpr size_t get_sub_result_global_offset(const size_t& idx) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			_match_value sub_result = _get_sub_result(idx);
			auto& value = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.first + sub_result.first;
		}
		constexpr size_t get_sub_result_relative_offset(const size_t& idx) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			_match_value sub_result = _get_sub_result(idx);
			auto& value = std::get<_value_result_data>(_data);
			return value.result.first + sub_result.first;
		}
		constexpr size_t get_sub_result_in_result_offset(const size_t& idx) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			return _get_sub_result(idx).first;
		}
		constexpr size_t get_sub_result_global_offset(const _string_type& name) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			_match_value sub_result = _get_sub_result(_get_named_sub_result_idx(name));
			auto& value = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.first + sub_result.first;
		}
		constexpr size_t get_sub_result_relative_offset(const _string_type& name) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			_match_value sub_result = _get_sub_result(_get_named_sub_result_idx(name));
			auto& value = std::get<_value_result_data>(_data);
			return value.result.first + sub_result.first;
		}
		constexpr size_t get_sub_result_in_result_offset(const _string_type& name) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return bad_offset;
			return _get_sub_result(_get_named_sub_result_idx(name)).first;
		}
		constexpr std::vector<size_t> get_sub_results_global_offsets() const noexcept {
			if (!has_value()) return {};

			auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
			for (const auto& [offset, subvalue] : value.sub_results) {
				offsets.push_back(value.search_offset + value.result.first + offset);
			}
			return offsets;
		}
		constexpr std::vector<size_t> get_sub_results_relative_offsets() const noexcept {
			if (!has_value()) return {};

			auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
			for (const auto& [offset, subvalue] : value.sub_results) {
				offsets.push_back(value.result.first + offset);
			}
			return offsets;
		}
		constexpr std::vector<size_t> get_sub_results_in_result_offsets() const noexcept {
			if (!has_value()) return {};

			auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<size_t> offsets;
			offsets.reserve(sub_results.size());
			for (const auto& [offset, subvalue] : sub_results) {
				offsets.push_back(offset);
			}
			return offsets;
		}
#pragma endregion OFFSETS

#pragma region VALUE
		constexpr _string_type get_sub_result_value(const size_t& idx) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return _string_type();
			return _get_sub_result(idx).second;
		}
		constexpr _string_type get_sub_result_value(const _string_type& name) const PCRE2CPP_NOEXCEPT {
			if (!has_value()) return _string_type();
			return _get_sub_result(_get_named_sub_result_idx(name)).second;
		}
		constexpr std::vector<_string_type> get_sub_results_values() const noexcept {
			if (!has_value()) return {};

			auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<_string_type> values;
			values.reserve(sub_results.size());
			for (const auto& [offset, subvalue] : sub_results) {
				values.push_back(subvalue);
			}
			return values;
		}
#pragma endregion VALUE
#pragma endregion SUB_RESULTS
		
#pragma endregion RESULTS
	};
}