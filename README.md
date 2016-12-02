# foreman

## Description

Simple, header-only, modern C++ unit-testing library. Librabry puts simple result data into `stdout` in UTF-8 encoding and may generate JSON result.

## Build instructions

The Library doen't require any special flags or other something. For example, for test program use `gcc -std=gnu++1z test.cpp -o test -lstd++ -static`.
Flag `-static` help for assembly in Windows (no dependencis for .dll). 

## Using

Add `#include <foreman.hpp>` to code file and add to project include directories path to headers. You may use many `Module()` in your code, in different headers. Library is cross-platform, but if you take some error, please send message or open issue.
For example (`test/test.cpp` in this repo):

```cpp
#include <cstdio>
#include <cstdint>

#include "foreman.hpp"

using namespace foreman;

Module(u8"Some module 1", {
    Test(u8"Some test 1", []() {
        Assert::isTrue([]() -> bool { return true; });
    });

    Test(u8"Some test 2", []() {
        Assert::isTrue([]() -> bool { return true; });
    });
});

Module(u8"Some module 2", {
    Test(u8"Some test 1", []() {
        Assert::isTrue([]() -> bool { return true; });
    });

    Test(u8"Some test 2", []() {
        Assert::isTrue([]() -> bool { return false; });
    });
});

int32_t main() {
    Core::getInstance().run();

    return 0;
}
```
This code puts, for example, this:
![foreman *nix console test output](https://github.com/emdc/foreman/master/doc/screenshot.png)
or
![foreman *nix console test output](https://github.com/emdc/foreman/master/doc/screenshot-cmder.png)

## Used libraries
* [termcolor](https://github.com/ikalnytskyi/termcolor) for colorize console output (BSD-3 license);
* [JSON for modern C++](https://github.com/nlohmann/json) for simple header-only support of JSON format (MIT license).

## License
The library is licensed under the [MIT License](http://opensource.org/licenses/MIT)

Copyright (c) 2016 [Eduard Pikhno](https://emdc.ru/) aka [emdc](https://github.com/emdc/)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
