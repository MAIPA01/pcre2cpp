/*
 * pcre2_data.hpp
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

namespace pcre2cpp {
	template<size_t utf> 
	struct _pcre2_data {};

	template<>
	struct _pcre2_data<8> {
		using code_type = pcre2_code_8;
		using string_ptr_type = PCRE2_SPTR8;
		using compile_ctx_type = pcre2_compile_context_8;
		using general_ctx_type = pcre2_general_context_8;
		using match_ctx_type = pcre2_match_context_8;
		using match_data_type = pcre2_match_data_8;
		using string_type = std::string;
		using string_char_type = char;
		using uchar_type = PCRE2_UCHAR8;

		static constexpr size_t utf = 8;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_8;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_8;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_8;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_8;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_8;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_8;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_8;
	};

	template<>
	struct _pcre2_data<16> {
		using code_type = pcre2_code_16;
		using string_ptr_type = PCRE2_SPTR16;
		using compile_ctx_type = pcre2_compile_context_16;
		using general_ctx_type = pcre2_general_context_16;
		using match_ctx_type = pcre2_match_context_16;
		using match_data_type = pcre2_match_data_16;
		using string_type = std::wstring;
		using string_char_type = wchar_t;
		using uchar_type = PCRE2_UCHAR16;

		static constexpr size_t utf = 16;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_16;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_16;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_16;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_16;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_16;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_16;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_16;
	};

	template<>
	struct _pcre2_data<32> {
		using code_type = pcre2_code_32;
		using string_ptr_type = PCRE2_SPTR32;
		using compile_ctx_type = pcre2_compile_context_32;
		using general_ctx_type = pcre2_general_context_32;
		using match_ctx_type = pcre2_match_context_32;
		using match_data_type = pcre2_match_data_32;
		using string_type = std::u32string;
		using string_char_type = char32_t;
		using uchar_type = PCRE2_UCHAR32;

		static constexpr size_t utf = 32;
		static constexpr code_type* (*compile)(string_ptr_type, size_t,
			uint32_t, int*, size_t*, compile_ctx_type*) = pcre2_compile_32;
		static constexpr match_data_type* (*match_data_from_pattern)
			(const code_type*, general_ctx_type*) = pcre2_match_data_create_from_pattern_32;
		static constexpr int (*match)(const code_type*, string_ptr_type,
			size_t, size_t, uint32_t, match_data_type*, match_ctx_type*) = pcre2_match_32;
		static constexpr size_t* (*get_ovector_ptr)(match_data_type*) = pcre2_get_ovector_pointer_32;
		static constexpr void(*match_data_free)(match_data_type*) = pcre2_match_data_free_32;
		static constexpr void(*code_free)(code_type*) = pcre2_code_free_32;
		static constexpr int(*get_error_message)(int, uchar_type*, size_t) = pcre2_get_error_message_32;
	};
}