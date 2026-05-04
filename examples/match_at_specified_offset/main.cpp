#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("\\d+");

	if (expression.match_at("aa2", 2)) { // is true
		cout << "Matches result: 2 at: 2" << endl;
	}

	if (expression.match_at("aa2", 1)) { // is false
		cout << "Matches result: 2 at: 2" << endl;
	}

	return 0;
}