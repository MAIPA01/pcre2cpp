#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("\\d+");

	std::vector<match_result> results;
	if (expression.match_all("Ala ma 23 lata i 3 koty", results)) { // is true
		cout << "Match 0 result: " << results[0].get_result_value()
		<< " at: " << results[0].get_result_global_offset()
		<< ", Match 1 result: " << results[1].get_result_value()
		<< " at: " << results[1].get_result_global_offset() << endl;

		// Should print: "Match 0 result: 23 at: 7, Match 1 result: 3 at: 17"
	}

	return 0;
}