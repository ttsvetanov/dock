#include <cstdio>
#include <cstdint>

#include "dock.hpp"

using namespace dock;

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
    nlohmann::json outJson;
    outJson[u8"version"] = u8"0.2.1";
    outJson[u8"date"] = __DATE__;
    outJson[u8"app"] = u8"dock-test";

    Core::getInstance().run();
    Core::getInstance().putResultsIntoJson(outJson);

    std::cout << std::endl;
    std::cout << outJson.dump(2);

    return 0;
}