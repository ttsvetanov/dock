#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <functional>
#include <vector>
#include <tuple>

#include "assert.hpp"
#include "format.hpp"

namespace foreman {
	class Core {
	public:
		static constexpr Core& getInstance() {
			return Core::instance;
		}

        void addModule(const char* moduleName, std::function<void()> moduleFunction);

        void run();
	private:
        static const size_t resizeStep = 100;
        static Core instance;

        Core() = default;

        std::vector<std::tuple<const char*, std::function<void()>>> modules;
	};

    class ModuleInternal {
    public:
        ModuleInternal(const char* name, std::function<void()> function);
    };

    void Test(const char* name, std::function<void()> function);

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    Core Core::instance;

    // ----------------------------------------------------------------------------------------------------------------

    void Core::addModule(const char* moduleName, std::function<void()> moduleFunction) {
        try {
            if (this->modules.capacity() == this->modules.size()) {
                this->modules.reserve(this->modules.capacity() + Core::resizeStep);
            }
        } catch (...) {
            Format::printAllocateError(moduleName);
            return;
        }

        this->modules.push_back(std::tuple<const char*, std::function<void()>>(moduleName, moduleFunction));
    }

    void Core::run() {
        for (auto iter = this->modules.begin(); iter != this->modules.end(); ++iter) {
            Format::printModuleName(std::get<0>(*iter));
            std::get<1>(*iter)();
        }
    }

    // ----------------------------------------------------------------------------------------------------------------

    ModuleInternal::ModuleInternal(const char* name, std::function<void()> function) {
        Core::getInstance().addModule(name, function);
    }

    #define FOREMAN_CONCAT_(x,y)        x##y
    #define FOREMAN_CONCAT(x,y)         FOREMAN_CONCAT_(x,y)
    #define GENERATE_MODULE(number)     FOREMAN_CONCAT(static foreman::ModuleInternal internalModule, __COUNTER__)
    #define Module(name, function)      GENERATE_MODULE(__COUNTER__)(name, function)

    // ----------------------------------------------------------------------------------------------------------------

	void Test(const char* name, std::function<void()> function) {
		try {
			function();
            Format::printPassedTest(name);
		} catch (Assert::Exception& e) {
            (void) e;
            Format::printFailedTest(name);
		}
	}
}