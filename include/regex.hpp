/*
 * regex.hpp
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 * This file is part of the pcre2cpp project, which is licensed under the BSD 3-Clause License with Attribution Requirement.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions, and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software must display the following acknowledgment:
 *    "This product includes software developed by Patryk Antosik (MAIPA01) as part of the pcre2cpp project."
 * 4. If you modify this software and distribute it, you must include a notice stating that the original work was created by Patryk Antosik (MAIPA01) as part of the pcre2cpp project.
 * 5. Neither the name of the author nor the name "pcre2cpp" may be used to endorse or promote products derived from this software without specific prior written permission.
 * 6. Sale of unmodified code: The unmodified version of this software may not be sold without the explicit permission of the author.
 * 7. Sale of modified code: Modified versions of the code may be sold, provided that:
 *    - Proper attribution to Patryk Antosik (MAIPA01) as the original author is maintained.
 *    - The modified code is clearly marked as a modified version.
 *    - The modified code must be accompanied by documentation that includes references to the original version and its author.
 * 8. Exception for large projects: If this code is used as part of a larger project (e.g., an application, framework, or other libraries), permission is not required for the sale of the unmodified version, as long as:
 *    - The project is more than just a wrapper for the original code.
 *    - The code is an integral part of the larger project, and the project does not merely involve selling the original code.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * PCRE2 Exception:
 * Notwithstanding the above, you are permitted to link or otherwise combine this software with the PCRE2 library (or a modified version of that library), and distribute the resulting binaries without restriction, provided that you do not modify the PCRE2 library itself.
 * Furthermore, the second condition in the BSD license (covering binary redistributions) does not apply to software that includes this wrapper as part of a larger binary package, unless the package itself uses PCRE2 independently.
 */

 /*
 * This file is part of the pcre2cpp project, which includes code from the PCRE2 library.
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 * This project is licensed under the BSD 3-Clause License with Attribution Requirement.
 *
 * PCRE2 library included in this project:
 * Copyright (c) 2016-2024, University of Cambridge.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notices, this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notices, this list of conditions, and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Cambridge nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * Exemption for binary redistributions applies as per PCRE2's BSD license terms.
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