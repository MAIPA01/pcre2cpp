/*
 * regex_exception.hpp
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
	class basic_regex_exception : public std::runtime_error {
	private:
		using string_type = _pcre2_data<utf>::string_type;
		using uchar_type = _pcre2_data<utf>::uchar_type;
		using char_type = _pcre2_data<utf>::string_char_type;

		string_type _message;

	public:
		basic_regex_exception(int error_code, size_t error_offset)
			: runtime_error(_generate_runtime_message(error_code, error_offset)),
				_message(_generate_message(error_code, error_offset)) { }

		string_type get_error() const {
			return _message;
		}

	private:
		static std::string _generate_runtime_message(int error_code, size_t error_offset) {
			std::string message = std::string("error at ").append(
				std::to_string(error_offset)).append(" ");

			uchar_type error_message[256];
			int size = _pcre2_data<utf>::get_error_message(error_code, error_message, 256);
			if (size != PCRE2_ERROR_BADDATA) {
				message.append(std::string(reinterpret_cast<const char*>(error_message),
					256 * (sizeof(uchar_type) / sizeof(unsigned char))));
			}
			return message;
		}

		static string_type _generate_message(int error_code, size_t error_offset) {
			string_type message = string_type("error at ").append(
				std::to_string(error_offset));

			if constexpr (utf == 8) {
				message.append(std::to_string(error_offset));
			}
			else if (utf == 16) {
				message.append(std::to_wstring(error_offset));
			}
			else if (utf == 32) {
				std::u32string value;
				for (wchar_t c : std::to_wstring(error_offset)) {
					value.append(1, c);
				}
				message.append(value);
			}
			message.append(" ");

			uchar_type error_message[256];
			int size = _pcre2_data<utf>::get_error_message(error_code, error_message, 256);
			if (size != PCRE2_ERROR_BADDATA) {
				message.append(string_type(reinterpret_cast<char_type*>(error_message), 256));
			}
			return message;
		}
	};
}