#include <cstdio>
#include <cstdint>

#include <typeinfo>

#include "foreman.hpp"

using namespace foreman;

int32_t main() {
    Module(u8"Some module", []() {
        Test(u8"Some test 1", []() {
            Assert::isTrue([]() -> bool { return true; });
        });

        Test(u8"Some test 2", []() {
            Assert::isTrue([]() -> bool { return true; });
        });

        Test(u8"Some test 3", []() {
            Assert::isTrue([]() -> bool { return false; });
        });
    });

	return 0;
}