# pcre2cpp

**pcre2cpp** is a C++ wrapper for the PCRE2 (Perl Compatible Regular Expressions) library written in C. It provides an object-oriented interface for the original PCRE2 library, while maintaining the same functionality and behavior.

## Features

- Object-oriented interface to PCRE2.
- Compatible with C++20.
- Easy to use regular expression matching with built-in result capturing.

## Installation

To use **pcre2cpp**, you need to link it statically with your project. Simply download the latest release and follow these steps:

1. Download the latest release of **pcre2cpp**.
2. Ensure that you have the PCRE2 library linked in your project (you will need it as a dependency).
3. Link the `pcre2cpp` library statically in your build system (e.g., `CMake` or `Make`).

Example for linking in `CMake`:
```cmake
find_package(pcre2cpp REQUIRED)
target_link_libraries(your_project_name PRIVATE pcre2cpp)
```

## Example Usage

### Basic Match

```cpp
#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
    regex expression("\\d+");
    
    if (expression.match("2")) {
        cout << "Matches" << endl;
    }
    
    return 0;
}
```

### Match with Result

```cpp
#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
    regex expression("\\d+");
    
    match_result result;
    if (expression.match("aa2", result, 2)) {
        cout << "Matches result: " << result.getValue() << " at: " 
             << to_string(result.getOffset() + 2) << endl;
    }
    
    return 0;
}
```

### Match at Specific Offset

```cpp
#include <pcre2cpp/pcre2cpp.hpp>
#include <iostream>

using namespace std;
using namespace pcre2cpp;

int main() {
    regex expression("\\d+");
    
    if (expression.match_at("aa2", 2)) {
        cout << "Matches at: 2" << endl;
    }
    
    return 0;
}
```

## Requirements

- C++20 or newer.
- PCRE2 library (https://github.com/PhilipHazel/pcre2).

## License

This project is licensed under the **BSD 3-Clause License with Attribution Requirement**. For more details, check the [LICENSE](./LICENSE) file.

## Acknowledgments

This project includes code from the [PCRE2 library](https://github.com/PhilipHazel/pcre2), distributed under the BSD License.