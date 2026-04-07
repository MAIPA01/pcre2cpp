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
		#pragma region MATCH_VALUE

	/**
	 * @brief Match value container
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	struct basic_match_value {
	private:
		using _string_type = typename utils::pcre2_data<utf>::string_type;

	public:
		/// @brief offset relative to search offset
		size_t relative_offset;
		/// @brief match value
		_string_type value;
	};

		#if _PCRE2CPP_HAS_UTF8
	using u8match_value = basic_match_value<utf_type::UTF_8>;
		#endif
		#if _PCRE2CPP_HAS_UTF16
	using u16match_value = basic_match_value<utf_type::UTF_16>;
		#endif
		#if _PCRE2CPP_HAS_UTF32
	using u32match_value = basic_match_value<utf_type::UTF_32>;
		#endif

		#if _PCRE2CPP_HAS_UTF8
	using match_value = u8match_value;
		#elif _PCRE2CPP_HAS_UTF16
	using match_value = u16match_value;
		#elif _PCRE2CPP_HAS_UTF32
	using match_value = u32match_value;
		#endif
		#pragma endregion

		#pragma region SUB_MATCH_VALUE

	/**
	 * @brief Sub match value container
	 * @ingroup pcre2cpp
	 */
	struct sub_match_value {
		/// @brief offset relative to search offset
		size_t relative_offset;
		/// @brief size of value
		size_t size;
	};

		#pragma endregion

	/**
	 * @brief Basic container to result data of match function
	 * @ingroup pcre2cpp
	 * @tparam utf UTF type
	 */
	template<utf_type utf>
	class basic_match_result {
	public:
		/// @brief error offset (returned when value doesn't exist or when error has occurred)
		static _PCRE2CPP_CONSTEXPR17 size_t bad_offset = std::numeric_limits<size_t>::max();

	private:
		using _pcre2_data_t		= utils::pcre2_data<utf>;
		using _string_type		= typename _pcre2_data_t::string_type;
		using _string_view_type = typename _pcre2_data_t::string_view_type;
		using _match_value		= basic_match_value<utf>;
		#if _PCRE2CPP_HAS_EXCEPTIONS
		using _match_result_exception = basic_match_result_exception<utf>;
		#endif
		using _named_sub_values_table	  = std::unordered_map<_string_type, size_t>;
		using _named_sub_values_table_ptr = std::shared_ptr<_named_sub_values_table>;

		/// @brief Result data container
		struct _value_result_data {
			size_t search_offset									 = bad_offset;
			_match_value result										 = { bad_offset, _string_type() };
			std::vector<std::optional<sub_match_value> > sub_results = {};
			_named_sub_values_table_ptr named_sub_values			 = {};
			bool found												 = false;
		};

		/// @brief Result data
		std::variant<match_error_codes, _value_result_data> _data = _value_result_data();

		/// @brief returns out of bounds error in correct utf format
		static _PCRE2CPP_CONSTEXPR17 _string_type _get_out_of_bounds_string() noexcept {
		#if _PCRE2CPP_HAS_UTF8
				if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) { return "Subexpression index out of bounds or has no value"; }
				else
		#endif
		#if _PCRE2CPP_HAS_UTF16
				  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) {
					return u"Subexpression index out of bounds or has no value";
				}
				else
		#endif
		#if _PCRE2CPP_HAS_UTF32
				  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) {
					return U"Subexpression index out of bounds or has no value";
				}
				else
		#endif
				{
					return _string_type();
				}
		}

		/// @brief returns subexpression not found error in correct utf format
		static _PCRE2CPP_CONSTEXPR17 _string_type _get_subexpression_not_found(const _string_view_type name) noexcept {
		#if _PCRE2CPP_HAS_UTF8
				if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_8) {
					return fmt::format("Subexpression with provided name '{}' not found", name);
				}
				else
		#endif
		#if _PCRE2CPP_HAS_UTF16
				  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_16) {
					return fmt::format(u"Subexpression with provided name '{}' not found", name);
				}
				else
		#endif
		#if _PCRE2CPP_HAS_UTF32
				  if _PCRE2CPP_CONSTEXPR17 (utf == utf_type::UTF_32) {
					return fmt::format(U"Subexpression with provided name '{}' not found", name);
				}
				else
		#endif
				{
					return _string_type();
				}
		}

		/// @brief returns true if sub result group with given name exists in named groups table
		_PCRE2CPP_CONSTEXPR17 bool _has_named_sub_result(const _string_view_type name) const noexcept {
			const auto& named_sub_values = std::get<_value_result_data>(_data).named_sub_values;
		#if _PCRE2CPP_HAS_CXX20
			return named_sub_values->contains(name.data());
		#else
			return named_sub_values->find(name.data()) != named_sub_values->end();
		#endif
		}

		/// @brief returns true if sub result has value and idx wasn't out of bounds
		_PCRE2CPP_CONSTEXPR17 bool _has_sub_value(const size_t idx) const noexcept {
			const auto& subResults = std::get<_value_result_data>(_data).sub_results;
			return idx < subResults.size() && subResults[idx].has_value();
		}

		/// @brief returns group index of group with given name
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

		/// @brief returns sub value data of group with provided index
		_PCRE2CPP_CONSTEXPR17 sub_match_value _get_sub_value(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!_has_sub_value(idx)) {
		#if _PCRE2CPP_HAS_EXCEPTIONS
					throw basic_match_result_exception<utf>(_get_out_of_bounds_string());
		#else
					pcre2cpp_assert(false, "{}", _get_out_of_bounds_string());
					return { .relative_offset = bad_offset, .size = 0 };
		#endif
				}

			return std::get<_value_result_data>(_data).sub_results[idx].value();
		}

	public:
		#pragma region CONSTRUCTORS
		/// @brief default constructor
		_PCRE2CPP_CONSTEXPR17 basic_match_result() noexcept = default;

		/// @brief constructor with error code
		_PCRE2CPP_CONSTEXPR17 explicit basic_match_result(const match_error_codes error_code) noexcept : _data(error_code) {}

		/// @brief constructor with no value but also without error
		_PCRE2CPP_CONSTEXPR17 basic_match_result(const size_t search_offset,
		  const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data { .search_offset = search_offset, .named_sub_values = named_sub_values }) {}

		/// @brief constructor with good result
		_PCRE2CPP_CONSTEXPR17 basic_match_result(const size_t search_offset, const _match_value& result,
		  const std::vector<std::optional<sub_match_value> >& sub_results,
		  const _named_sub_values_table_ptr& named_sub_values) noexcept
			: _data(_value_result_data { .search_offset = search_offset,
				  .result								= result,
				  .sub_results							= sub_results,
				  .named_sub_values						= named_sub_values,
				  .found								= true }) {}

		/// @brief default copy constructor
		_PCRE2CPP_CONSTEXPR17 basic_match_result(const basic_match_result& other) noexcept			  = default;
		/// @brief default move constructor
		_PCRE2CPP_CONSTEXPR17 basic_match_result(basic_match_result&& other) noexcept				  = default;
		#pragma endregion

		/// @brief default destructor
		_PCRE2CPP_CONSTEXPR20 ~basic_match_result() noexcept										  = default;

		/// @brief default copy assign operator
		_PCRE2CPP_CONSTEXPR17 basic_match_result& operator=(const basic_match_result& other) noexcept = default;
		/// @brief default move assign operator
		_PCRE2CPP_CONSTEXPR17 basic_match_result& operator=(basic_match_result&& other) noexcept	  = default;

		#pragma region ERRORS

		/// @brief returns true if result holds error
		_PCRE2CPP_CONSTEXPR17 bool has_error() const noexcept { return std::holds_alternative<match_error_codes>(_data); }

		/// @brief return error code
		_PCRE2CPP_CONSTEXPR17 match_error_codes get_error_code() const noexcept {
				if (!has_error()) { return match_error_codes::None; }
			return std::get<match_error_codes>(_data);
		}

		/// @brief returns error message
		_PCRE2CPP_CONSTEXPR17 _string_type get_error_message() const noexcept {
				if (!has_error()) { return _string_type(); }
			return pcre2cpp::generate_error_message<utf>(static_cast<int>(std::get<match_error_codes>(_data)));
		}

		#if _PCRE2CPP_HAS_EXCEPTIONS
		/// @brief throws error if result holds error with error message based on error code
		_PCRE2CPP_CONSTEXPR17 void throw_error() const {
				if (!has_error()) { return; }
			throw _match_result_exception(static_cast<int>(std::get<match_error_codes>(_data)));
		}
		#endif

		#pragma endregion ERRORS

		#pragma region RESULTS

		/// @brief returns true when result holds some result not error
		_PCRE2CPP_CONSTEXPR17 bool has_result() const noexcept { return std::holds_alternative<_value_result_data>(_data); }

		/// @brief returns true when result has value
		_PCRE2CPP_CONSTEXPR17 bool has_value() const noexcept {
				if (!has_result()) { return false; }
			return std::get<_value_result_data>(_data).found;
		}

		/// @brief returns true when result has sub value on given index
		_PCRE2CPP_CONSTEXPR17 bool has_sub_value(const size_t idx) const noexcept {
				if (!has_value()) { return false; }
			return _has_sub_value(idx);
		}

		/// @brief returns true when result has sub value with given name
		_PCRE2CPP_CONSTEXPR17 bool has_sub_value(const _string_view_type name) const noexcept {
			return _has_named_sub_result(name) && has_sub_value(_get_named_sub_result_idx(name));
		}

		/// @brief returns search offset
		_PCRE2CPP_CONSTEXPR17 size_t get_search_offset() const noexcept {
				if (!has_result()) { return bad_offset; }
			return std::get<_value_result_data>(_data).search_offset;
		}

		#pragma region RESULT

		/// @brief returns match result container
		_PCRE2CPP_CONSTEXPR17 _match_value get_result() const noexcept {
				if (!has_value()) { return { bad_offset, _string_type() }; }
			return std::get<_value_result_data>(_data).result;
		}

		/// @brief returns offset of value from the beginning of searched string
		_PCRE2CPP_CONSTEXPR17 size_t get_result_global_offset() const noexcept {
				if (!has_value()) { return bad_offset; }
			const auto& value = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.relative_offset;
		}

		/// @brief return offset relative to search offset
		_PCRE2CPP_CONSTEXPR17 size_t get_result_relative_offset() const noexcept {
				if (!has_value()) { return bad_offset; }
			return std::get<_value_result_data>(_data).result.relative_offset;
		}

		/// @brief returns size of match value
		_PCRE2CPP_CONSTEXPR17 size_t get_result_size() const noexcept {
				if (!has_value()) { return 0; }
			return std::get<_value_result_data>(_data).result.value.size();
		}

		/// @brief returns match string value
		_PCRE2CPP_CONSTEXPR17 _string_type get_result_value() const noexcept {
				if (!has_value()) { return _string_type(); }
			return std::get<_value_result_data>(_data).result.value;
		}

		#pragma endregion RESULT

		#pragma region ALL_SUB_RESULTS

		/// @brief returns all sub results
		_PCRE2CPP_CONSTEXPR20 std::vector<std::optional<sub_match_value> > get_sub_results() const noexcept {
				if (!has_value()) { return {}; }
			return std::get<_value_result_data>(_data).sub_results;
		}

		/// @brief returns sub results count
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_results_count() const noexcept { return get_sub_results().size(); }

		/// @brief returns sub results offsets from the beginning of search string
		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_global_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
				for (const auto& subResult : value.sub_results) {
						if (subResult.has_value()) {
							offsets.push_back(value.search_offset + value.result.relative_offset + subResult->relative_offset);
						}
						else { offsets.push_back(bad_offset); }
				}
			return offsets;
		}

		/// @brief returns sub results offsets relative to search offset
		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_relative_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& value = std::get<_value_result_data>(_data);

			std::vector<size_t> offsets;
			offsets.reserve(value.sub_results.size());
				for (const auto& subResult : value.sub_results) {
						if (subResult.has_value()) {
							offsets.push_back(value.result.relative_offset + subResult->relative_offset);
						}
						else { offsets.push_back(bad_offset); }
				}
			return offsets;
		}

		/// @brief returns sub results offsets relative to result offset
		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_in_result_offsets() const noexcept {
				if (!has_value()) { return {}; }

			const auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<size_t> offsets;
			offsets.reserve(sub_results.size());
				for (const auto& subResult : sub_results) {
						if (subResult.has_value()) { offsets.push_back(subResult->relative_offset); }
						else { offsets.push_back(bad_offset); }
				}
			return offsets;
		}

		/// @brief returns sub results value sizes
		_PCRE2CPP_CONSTEXPR20 std::vector<size_t> get_sub_results_sizes() const noexcept {
				if (!has_value()) { return {}; }

			const auto& sub_results = std::get<_value_result_data>(_data).sub_results;

			std::vector<size_t> values;
			values.reserve(sub_results.size());
				for (const auto& subResult : sub_results) {
						if (subResult.has_value()) { values.emplace_back(subResult->size); }
						else { values.push_back(0); }
				}
			return values;
		}

		/// @brief returns sub results string values
		_PCRE2CPP_CONSTEXPR17 std::vector<_string_type> get_sub_results_values() const noexcept {
				if (!has_value()) { return {}; }

			const auto& data		= std::get<_value_result_data>(_data);
			const auto& value		= data.result.value;
			const auto& sub_results = data.sub_results;

			std::vector<_string_type> values;
			values.reserve(sub_results.size());
				for (const auto& subResult : sub_results) {
						if (subResult.has_value()) {
							values.emplace_back(value.data() + subResult->relative_offset, subResult->size);
						}
						else { values.push_back(_string_type()); }
				}
			return values;
		}

		#pragma endregion ALL_SUB_RESULTS

		#pragma region SUB_RESULTS_BY_IDX

		/// @brief returns sub result container
		_PCRE2CPP_CONSTEXPR17 sub_match_value get_sub_result(const size_t idx) const _PCRE2CPP_NOEXCEPT {
			return _get_sub_value(idx);
		}

		/// @brief returns sub result offset from the beginning of searched string
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_global_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_sub_value(idx)) { return bad_offset; }

			const auto [relative_offset, size] = _get_sub_value(idx);
			const auto& value				   = std::get<_value_result_data>(_data);
			return value.search_offset + value.result.relative_offset + relative_offset;
		}

		/// @brief returns sub result offset relative to search offset
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_relative_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_sub_value(idx)) { return bad_offset; }

			const auto [relative_offset, size] = _get_sub_value(idx);
			const auto& value				   = std::get<_value_result_data>(_data);
			return value.result.relative_offset + relative_offset;
		}

		/// @brief returns sub result offset relative to result offset
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_in_result_offset(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_sub_value(idx)) { return bad_offset; }
			return _get_sub_value(idx).relative_offset;
		}

		/// @brief returns sub result value size
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_size(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_sub_value(idx)) { return 0; }
			return _get_sub_value(idx).size;
		}

		/// @brief returns sub result string value
		_PCRE2CPP_CONSTEXPR17 _string_type get_sub_result_value(const size_t idx) const _PCRE2CPP_NOEXCEPT {
				if (!has_sub_value(idx)) { return _string_type(); }

			const auto [relative_offset, size] = _get_sub_value(idx);
			const auto& value				   = std::get<_value_result_data>(_data).result.value;
			return _string_type(value.data() + relative_offset, size);
		}

		#pragma endregion

		#pragma region SUB_RESULTS_BY_NAME

		/// @brief returns sub result container
		_PCRE2CPP_CONSTEXPR17 sub_match_value get_sub_result(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result(_get_named_sub_result_idx(name));
		}

		/// @brief returns sub result offset from the beginning of searched string
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_global_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_global_offset(_get_named_sub_result_idx(name));
		}

		/// @brief returns sub result offset relative to search offset
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_relative_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_relative_offset(_get_named_sub_result_idx(name));
		}

		/// @brief returns sub result offset relative to result offset
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_in_result_offset(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_in_result_offset(_get_named_sub_result_idx(name));
		}

		/// @brief returns sub result value size
		_PCRE2CPP_CONSTEXPR17 size_t get_sub_result_size(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_size(_get_named_sub_result_idx(name));
		}

		/// @brief returns sub result string value
		_PCRE2CPP_CONSTEXPR17 _string_type get_sub_result_value(const _string_view_type name) const _PCRE2CPP_NOEXCEPT {
			return get_sub_result_value(_get_named_sub_result_idx(name));
		}

		#pragma endregion

		#pragma endregion RESULTS
	};

		#if _PCRE2CPP_HAS_UTF8
	using u8match_result = basic_match_result<utf_type::UTF_8>;
		#endif
		#if _PCRE2CPP_HAS_UTF16
	using u16match_result = basic_match_result<utf_type::UTF_16>;
		#endif
		#if _PCRE2CPP_HAS_UTF32
	using u32match_result = basic_match_result<utf_type::UTF_32>;
		#endif

		#if _PCRE2CPP_HAS_UTF8
	using match_result = u8match_result;
		#elif _PCRE2CPP_HAS_UTF16
	using match_result = u16match_result;
		#elif _PCRE2CPP_HAS_UTF32
	using match_result = u32match_result;
		#endif
} // namespace pcre2cpp
	#endif
#endif