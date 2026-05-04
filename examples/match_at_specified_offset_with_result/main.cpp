#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("\\d+");

	match_result result;
	if (expression.match_at("aa2", result, 2)) { // is true
		cout << "Matches result: " << result.get_result_value() << " at: "
			 << to_string(result.get_result_global_offset()) << endl;

		// Should print: "Matches result: 2 at: 2"
	}

	return 0;
}