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
#ifndef _PCRE2CPP_MATCH_RESULT_HPP_
	#define _PCRE2CPP_MATCH_RESULT_HPP_

	#include <pcre2cpp/config.hpp>

	#if !_PCRE2CPP_HAS_CXX17
_PCRE2CPP_ERROR("This is only available for c++17 and greater!");
	#else

		#include <pcre2cpp/exceptions/exceptions.hpp>
		#include <pcre2cpp/match/match_error_codes.hpp>
		#include <pcre2cpp/types.hpp>
		#include <pcre2cpp/utils/pcre2_data.hpp>

namespace pcre2cpp {
	template<size_t utf>
	struct basic_match_value {
		using pcre2_data_t = pcre2_data<utf>;
		using string_type  = typename pcre2_data_t::string_type;

		size_t relative_offset;
		string_type value;
	};

	template<size_t utf>
	class basic_match_result {
	public:
		static _PCRE2CPP_CONSTEXPR17 size_t bad_offset = std::numeric_limits<size_t>::max();

	private:
		using _pcre2_data_t		= pcre2_data<utf>;
		using _string_type		= typename _pcre2_data_t::string_type;
		using _string_view_type = typename _pcre2_data_t::string_view_type;
		using _match_value		= basic_match_value<utf>;
		#if _PCRE2CPP_HAS_EXCEPTIONS
		using _match_result_exception = basic_match_result_exception<utf>;
		#endif
		using _named_sub_values_table	  = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;

		struct _value_result_data {
			size_t search_offset						 = bad_offset;
			_match_value result							 = { bad_offset, _string_type() };
			std::vector<_match_value> sub_results		 = {};
			_named_sub_values_table_ptr named_sub_values = {};
			bool found									 = false;
		};

		std::variant<match_error_codes, _value_result_data> _data = _value_result_data();

		static _PCRE2CPP_CONSTEXPR17 _string_type _get_out_of_bounds_string() noexcept {
				if _PCRE2CPP_CONSTEXPR17 (utf == 8) { return "Subexpression index out of bounds"; }
				else if _PCRE2CPP_CONSTEXPR17 (utf == 16) { return u"Subexpression index out of bounds"; }
				else if _PCRE2CPP_CONSTEXPR17 (utf == 32) { return U"Subexpression index out of bounds"; }
				else { return _string_type(); }
		}

		static _PCRE2CPP_CONSTEXPR17 _string_type _get_subexpression_not_found(const _string_view_type name) noexcept {
				if _PCRE2CPP_CONSTEXPR17 (utf == 8) {
					return fmt::format("Subexpression with provided name '{}' not found", name);
				}
				else if _PCRE2CPP_CONSTEXPR17 (utf == 16) {
					return fmt::format(u"Subexpression with provided name '{}' not found", name);
				}
				else if _PCRE2CPP_CONSTEXPR17 (utf == 32) {
					return fmt::format(U"Subexpression with provided name '{}' not found", name);
				}
				else { return _string_type(); }
		}

		_PCRE2CPP_CONSTEXPR17 bool _has_named_sub_result(const _string_view_type name) const noexcept {
			const auto& named_sub_values = std::get<_value_result_data>(_data).named_sub_values;
		#if _PCRE2CPP_HAS_CXX20
			return named_sub_values->contains(name.data());
		#else
			return named_sub_values->find(name.data()) != named_sub_values->end();
		#endif
		}

		_PCRE2CPP_CONSTEXPR17 bool _has_sub_result(const size_t idx) const noexcept {
			return idx < std::get<_value_result_data>(_data).sub_results.size();
		}

		_PCRE2CPP_CONSTEXPR17 size_t _get_named_sub_result_idx(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
				if (!_has_named_sub_result(name)) {
		#if _PCRE2CPP_HAS_EXCEPTIONS
					throw _match_result_exception(_get_subexpression_not_found(name));
		#else
					pcre2cpp_assert(false, "{}", _get_subexpression_not_found(name));
					return bad_offset;
		#endif
				}
			return std::get<_value_result_data>(_data).named_sub_values->at(name.data());
		}

		_PCRE2CPP_CONSTEXPR17 _match_value _get_sub_result(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!_has_sub_result(idx)) {
		#if _PCRE2CPP_HAS_EXCEPTIONS
					throw basic_match_result_exception<utf>(_get_out_of_bounds_string());
		#else
					pcre2cpp_assert(false, "{}", _get_out_of_bounds_string());
					return { .relative_offset = bad_offset, .value = _string_type() };
		#endif
				}

			return std::get<_value_result_data>(_data).sub_results[idx];
		}

	public:
		_PCRE2CPP_CONSTEXPR17 basic_match_result() noexcept = default;

		_PCRE2CPP_CONSTEXPR17 explicit basic_match_result(const match_error_codes error_code) noexcept : _data(error_code) {}

		_PCRE2CPP_CONSTEXPR17 basic_match_result(const size_t search_offset,
		  const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data {
				  search_offset, { .relative_offset = bad_offset, .value = _string_type() },
					 {},
					 named_sub_values, false
		 }) {}

		_PCRE2CPP_CONSTEXPR17 basic_match_result(const size_t search_offset, const _match_value& result,
		  const std::vector<_match_value>& sub_results, const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data { search_offset, result, sub_results, named_sub_values, true }) {}

		_PCRE2CPP_CONSTEXPR17 basic_match_result(const basic_match_result& other) noexcept			  = default;
		_PCRE2CPP_CONSTEXPR17 basic_match_result(basic_match_result&& other) noexcept				  = default;

		_PCRE2CPP_CONSTEXPR20 ~basic_match_result() noexcept										  = default;

		_PCRE2CPP_CONSTEXPR17 basic_match_result& operator=(const basic_match_result& other) noexcept = default;
		_PCRE2CPP_CONSTEXPR17 basic_match_result& operator=(basic_match_result&& other) noexcept	  = default;

		#pragma region ERRORS

		_PCRE2CPP_CONSTEXPR17 bool has_error() const noexcept { return std::holds_alternative<int>(_data); }

		_PCRE2CPP_CONSTEXPR17 match_error_codes get_error_code() const noexcept {
				if (!has_error()) { return match_error_codes::None; }
			return std::get<int>(_data);
		}

		_PCRE2CPP_CONSTEXPR17 _string_type get_error_message() const noexcept {
				if (!has_error()) { return _string_type(); }
			return pcre2cpp::generate_error_message<utf>(std::get<int>(_data));
		}

		#if _PCRE2CPP_HAS_EXCEPTIONS
		_PCRE2CPP_CONSTEXPR17 void throw_error() const {
				if (!has_error()) { return; }
			throw _match_result_exception(std::get<int>(_data));
		}
		#endif

		#pragma endregion ERRORS

		#pragma region RESULTS

		_PCRE2CPP_CONSTEXPR17 bool has_result() const noexcept { return std::holds_alternative<_value_result_data>(_data); }

		_PCRE2CPP_CONSTEXPR17 bool has_value() const noexcept {
				if (!has_result()) { return false; }
			return std::get<_value_result_data>(_data).found;
		}

		_PCRE2CPP_CONSTEXPR17 bool has_sub_value(const size_t idx) const noexcept { return _has_sub_result(idx); }

		_PCRE2CPP_CONSTEXPR17 bool has_sub_value(const _string_view_type name) const noexcept {
			return _has_named_sub_result(name) && has_sub_value(_get_named_sub_result_idx(name));
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_search_offset() const noexcept {
				if (!has_result()) { return bad_offset; }
			return std::get<_value_result_data>(_data).search_offset;
		}

		#pragma region RESULT

		_PCRE2CPP_CONSTEXPR17 size_t get_result_global_offset() const noexcept {
				if (!has_value()) { return bad_offset; }
			const auto& value = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.relative_offset;
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_result_relative_offset() const noexcept {
				if (!has_value()) { return bad_offset; }
			return std::get<_value_result_data>(_data).result.relative_offset;
		}

		_PCRE2CPP_CONSTEXPR17 _string_type get_result_value() const noexcept {
				if (!has_value()) { return _string_type(); }
			return std::get<_value_result_data>(_data).result.value;
		}

		#pragma endregion RESULT

		#pragma region ALL_SUB_RESULTS

		_PCRE2CPP_CONSTEXPR17 std::vector<_match_value> get_sub_results() const noexcept {
				if (!has_value()) { return {}; }
			return std::get<_value_result_data>(_data).sub_results;
		}

		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_global_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
				for (const auto& [offset, subvalue] : value.sub_results) {
					offsets.push_back(value.search_offset + value.result.relative_offset + offset);
				}
			return offsets;
		}

		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_relative_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
				for (const auto& [offset, subvalue] : value.sub_results) { offsets.push_back(value.result.relative_offset + offset); }
			return offsets;
		}

		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_in_result_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<size_t> offsets;
			offsets.reserve(sub_results.size());
				for (const auto& [offset, subvalue] : sub_results) { offsets.push_back(offset); }
			return offsets;
		}

		_PCRE2CPP_CONSTEXPR17 std::vector<_string_type> get_sub_results_values() const noexcept {
				if (!has_value()) { return {}; }

			const auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<_string_type> values;
			values.reserve(sub_results.size());
				for (const auto& [offset, subvalue] : sub_results) { values.push_back(subvalue); }
			return values;
		}

		#pragma endregion ALL_SUB_RESULTS

		#pragma region SUB_RESULTS_BY_IDX

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_global_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_value()) { return bad_offset; }
			_match_value sub_result = _get_sub_result(idx);
			const auto& value		= std::get<_value_result_data>(_data);
			return value.search_offset + value.result.relative_offset + sub_result.relative_offset;
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_relative_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_value()) { return bad_offset; }
			_match_value sub_result = _get_sub_result(idx);
			const auto& value		= std::get<_value_result_data>(_data);
			return value.result.relative_offset + sub_result.relative_offset;
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_in_result_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_value()) { return bad_offset; }
			return _get_sub_result(idx).relative_offset;
		}

		_PCRE2CPP_CONSTEXPR17 _string_type get_sub_result_value(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_value()) { return _string_type(); }
			return _get_sub_result(idx).value;
		}

		#pragma endregion

		#pragma region SUB_RESULTS_BY_NAME

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_global_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_global_offset(_get_named_sub_result_idx(name));
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_relative_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_relative_offset(_get_named_sub_result_idx(name));
		}

		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_in_result_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_in_result_offset(_get_named_sub_result_idx(name));
		}

		_PCRE2CPP_CONSTEXPR17 _string_type get_sub_result_value(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_value(_get_named_sub_result_idx(name));
		}

		#pragma endregion

		#pragma endregion RESULTS
	};
} // namespace pcre2cpp
	#endif
#endif