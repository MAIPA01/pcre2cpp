#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("(?<number>\\d+)(?<a>a)");

	match_result result;
	if (expression.match("ab23a", result, 1)) { // is true
		cout << "Sub Match <number> result: " << result.get_sub_result_value("number")
		<< " at: " << result.get_sub_result_global_offset("number")
		<< ", Sub Match <a> result: " << result.get_sub_result_value("a")
		<< " at: " << result.get_sub_result_global_offset("a") << endl;

		// Should print: "Sub Match <number> result: 23 at: 2, Sub Match <a> result: a at: 4"
	}

	return 0;
}