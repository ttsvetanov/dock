#include <cstdio>
#include <cstdint>

#include <dock.hpp>
#include <serializers/json_serializer.hpp>
#include <serializers/text_serializer.hpp>
#include <serializers/console_serializer.hpp>

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

    JsonSerializer      jsonSerializer(outJson, 4);
    TextSerializer      textSerializer;
    ConsoleSerializer   consoleSerializer;

    outJson[u8"version"] = u8"0.3.0";
    outJson[u8"date"] = __DATE__;
    outJson[u8"app"] = u8"dock-test";

    core().run();
    core().collect(jsonSerializer);
    core().collect(textSerializer);
    core().collect(consoleSerializer);

    outJson[u8"passed_count"] = core().getPassedCount();
    outJson[u8"tests_count"] = core().getTestsCount();
    outJson[u8"modules_count"] = core().getModulesCount();

    std::cout << "JSON:\n";
    std::cout << jsonSerializer << std::endl;
    std::cout << "\nPlain text:\n";
    std::cout << textSerializer << std::endl;
    std::cout << "\nConsole out:\n";
    std::cout << consoleSerializer << std::endl;

    return 0;
}