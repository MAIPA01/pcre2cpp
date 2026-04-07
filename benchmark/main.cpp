#include <pch.hpp>

void benchmark(const std::string_view name, const mstd::function_view<void()> func, const size_t iterations) {
	const auto start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < iterations; ++i) { func(); }
	const auto end											= std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << fmt::format("[[BENCHMARK]] {} ({} iterations):", name, iterations) << elapsed.count() << " ms" << std::endl;
}

int main() {
	const std::string pattern = R"((\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*))";
	const std::string subject = "Moj adres to test.email@example.com, a inny to kontakt@firma.pl";
	constexpr int iterations  = 100'00;

	std::cout << "Starting benchmark...\n" << std::endl;

#pragma region COMPILATION_BENCHMARK

	// --- 1. std::regex compilation ---
	benchmark("std::regex compilation", [&]() -> void { const std::regex std_re(pattern); }, iterations);

	// --- 2. PCRE2 C compilation ---
	benchmark(
	  "PCRE2 C compilation",
	  [&]() -> void {
		  int errorNumber;
		  PCRE2_SIZE errorOffset;
		  pcre2_code_8* re_c = pcre2_compile_8(reinterpret_cast<PCRE2_SPTR8>(pattern.c_str()), pattern.length(), 0, &errorNumber,
			&errorOffset, nullptr);
		  pcre2_match_data_8* match_data = pcre2_match_data_create_from_pattern_8(re_c, nullptr);

		  pcre2_match_data_free_8(match_data);
		  pcre2_code_free_8(re_c);
	  },
	  iterations
	);

	// --- 3. pcre2cpp::regex compilation ---
	benchmark(
	  "pcre2cpp::regex compilation", [&]() -> void { const pcre2cpp::regex pcre2cpp_re(pattern); }, iterations
	);

#pragma endregion

	// COMPILED std::regex
	const std::regex std_re(pattern);

	// COMPILED PCRE2 C
	int errorNumber;
	PCRE2_SIZE errorOffset;
	pcre2_code_8* re_c =
	  pcre2_compile_8(reinterpret_cast<PCRE2_SPTR8>(pattern.c_str()), pattern.length(), 0, &errorNumber, &errorOffset, nullptr);
	pcre2_match_data_8* match_data = pcre2_match_data_create_from_pattern_8(re_c, nullptr);

	// COMPILED pcre2cpp::regex
	const pcre2cpp::regex pcre2cpp_re(pattern);

#pragma region MATCH_BENCHMARK

	// --- 1. std::regex match ---
	benchmark(
	  "std::regex match",
	  [&]() -> void {
		  std::smatch match;
		  std::regex_search(subject, match, std_re);
	  },
	  iterations
	);

	// --- 2. PCRE2 C match ---
	benchmark(
	  "PCRE2 C match",
	  [&]() -> void {
		  pcre2_match_8(re_c, reinterpret_cast<PCRE2_SPTR8>(subject.c_str()), subject.length(), 0, 0, match_data, nullptr);
	  },
	  iterations
	);

	// --- 3. pcre2cpp::regex match ---
	benchmark("pcre2cpp::regex match", [&]() -> void { std::ignore = pcre2cpp_re.match(subject); }, iterations);

#pragma endregion

#pragma region MATCH_WITH_ALL_RESULTS

	// --- 1. std::regex match with all results ---
	benchmark(
	  "std::regex match with all results",
	  [&]() -> void {
		  std::smatch match;
		  std::regex_search(subject, match, std_re);
		  const std::string result = match.str();
	  },
	  iterations
	);

	// --- 2. PCRE2 C match with all results ---
	benchmark(
	  "PCRE2 C match with all results",
	  [&]() -> void {
		  const int errorCode =
			pcre2_match_8(re_c, reinterpret_cast<PCRE2_SPTR8>(subject.c_str()), subject.length(), 0, 0, match_data, nullptr);
			  if (errorCode == PCRE2_ERROR_NOMATCH || errorCode <= 0) { return; }

		  const size_t* offsetVector	  = pcre2_get_ovector_pointer_8(match_data);
		  const size_t offsetVectorsCount = pcre2_get_ovector_count_8(match_data);

		  auto results					  = std::vector<std::string>();
		  results.reserve(offsetVectorsCount);
			  for (size_t i = 0; i < offsetVectorsCount; ++i) {
				  const size_t matchStart = offsetVector[i * 2];
				  const size_t matchEnd	  = offsetVector[i * 2 + 1];

					  if (matchStart == PCRE2_UNSET || matchEnd == PCRE2_UNSET) { results.emplace_back(); }
					  else { results.emplace_back(std::next(subject.begin(), matchStart), std::next(subject.begin(), matchEnd)); }
			  }
	  },
	  iterations
	);

	// --- 3. pcre2cpp::regex match ---
	benchmark(
	  "pcre2cpp::regex match with all results",
	  [&]() -> void {
		  pcre2cpp::match_result match;
			  if (!pcre2cpp_re.match(subject, match)) { return; }

		  std::vector<std::string> results = match.get_sub_results_values();
		  results.insert(results.begin(), match.get_result_value());
	  },
	  iterations
	);

#pragma endregion

	// CLEAN UP
	pcre2_match_data_free_8(match_data);
	pcre2_code_free_8(re_c);

	return 0;
}