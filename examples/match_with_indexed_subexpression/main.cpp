#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("(\\d+)(a)");

	match_result result;
	if (expression.match("ab23a", result, 1)) { // is true
		cout << "Sub Match 0 result: " << result.get_sub_result_value(0) << " at: "
		<< result.get_sub_result_global_offset(0) << ", Sub Match 1 result: "
		<< result.get_sub_result_value(1) << " at: "
		<< result.get_sub_result_global_offset(1) << endl;

		// Should print: "Sub Match 0 result: 23 at: 2, Sub Match 1 result: a at: 4"
	}

	return 0;
}