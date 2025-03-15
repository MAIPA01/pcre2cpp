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
	class basic_match_result {
	private:
		using _string_type = _pcre2_data<utf>::string_type;

		size_t _error_code = 0;
		size_t _offset = 0;
		_string_type _value = _string_type();

	public:
		constexpr basic_match_result() = default;
		constexpr basic_match_result(size_t error_code)
			: _error_code(error_code) {
		}
		constexpr basic_match_result(size_t offset, const _string_type& value)
			: _offset(offset), _value(value) {
		}
		constexpr basic_match_result(const basic_match_result<utf>& mr)
			: _error_code(mr._error_code), _offset(mr._offset), _value(mr._value) {
		}
		~basic_match_result() = default;

		constexpr basic_match_result<utf>& operator=(const basic_match_result<utf>& mr) {
			this->_error_code = mr._error_code;
			this->_offset = mr._offset;
			this->_value = mr._value;
			return *this;
		}

		constexpr size_t getErrorCode() const { return _error_code; }
		constexpr size_t getOffset() const { return _offset; }
		constexpr _string_type getValue() const { return _value; }
	};
}