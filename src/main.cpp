#include <cstdio>
#include <cstdint>

#include "foreman.hpp"

using namespace foreman;

Module(u8"Some module 1", []() {
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

Module(u8"Some module 2", []() {
    Test(u8"Some test 4", []() {
        Assert::isTrue([]() -> bool { return true; });
    });

    Test(u8"Some test 5", []() {
        Assert::isTrue([]() -> bool { return true; });
    });

    Test(u8"Some test 6", []() {
        Assert::isTrue([]() -> bool { return false; });
    });
});

int32_t main() {
    Core::getInstance().run();

    fflush(stdin);
    getchar();

	return 0;
}