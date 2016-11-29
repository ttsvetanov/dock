#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <functional>
#include <vector>
#include <tuple>

#include "assert.hpp"
#include "../third_party/termcolor.hpp"

namespace foreman {
	class Core {
	public:
		static Core& getInstance() {
			return Core::instance;
		}

        void collect() {}
	private:
        static Core instance;

		Core();
	};

    Core Core::instance;

	Core::Core() {
        #if defined(_WIN32) || defined(_WIN64)
		system("chcp 65001");
        #endif
	}

	// ----------------------------------------------------------------------------------------------------------------

	static void Module(const char* moduleName, std::function<void()> moduleFunction) {
		std::cout << u8"  " << moduleName << u8"\n";
		moduleFunction();
	}

	void Test(const char* testName, std::function<void()> testFunction) {
		try {
			testFunction();
			std::cout << termcolor::green << u8"    \u2713" << termcolor::white << " \"" << testName <<  u8"\": passed!\n";
		} catch (Assert::Exception& e) {
			std::cout << termcolor::red << u8"    x" << termcolor::white << " \"" << testName << u8"\": fail!\n";
			std::cout << u8"    Result: \"" << e.what() << u8"\"\n";
			std::flush(std::cout);
		}
	}
}