#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
	regex expression("\\d+");

	if (expression.match("2")) { // is true
		cout << "Matches" << endl;
	}

	if (expression.match("a")) { // is false
		cout << "Matches" << endl;
	}

	if (expression.match("a2")) { // is true
		cout << "Matches" << endl;
	}

	return 0;
}