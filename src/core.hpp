#pragma once

#include <cstdint>
#include <cstddef>

#include <functional>
#include <vector>
#include <map>
#include <tuple>

#include "assert.hpp"
#include "format.hpp"
#include "result.hpp"

namespace foreman {
    class Core;

    namespace _internal {
        class Module {
        public:
            Module(const char* name, std::function<void(Module*)> function);

            const char* getName() const { return this->moduleName; }

            void        addTest(const char* name, std::function<void()> func);
            void        runTests();
        private:
            const char* moduleName;

            std::map<const char*, std::function<void()>> tests;
        };        
    }

    class Core {
    public:
        static constexpr Core& getInstance() {
            return Core::instance;
        }

        void addModule(_internal::Module* module);

        void run();
    private:
        static const size_t resizeStep = 100;
        static Core instance;

        Core() = default;

        std::vector<_internal::Module*> modules;
    };

    // ----------------------------------------------------------------------------------------------------------------

    Core Core::instance;

    // ----------------------------------------------------------------------------------------------------------------

    void Core::addModule(_internal::Module* module) {
        if (module == nullptr) {
            return;
        }

        try {
            if (this->modules.capacity() == this->modules.size()) {
                this->modules.reserve(this->modules.capacity() + Core::resizeStep);
            }
            this->modules.push_back(module);
        } catch (...) {
            Format::printAllocateError(module->getName());
            return;
        }
    }

    void Core::run() {
        for (auto iter = this->modules.begin(); iter != this->modules.end(); ++iter) {
            if (*iter) {
                Format::printModuleName((*iter)->getName());
                (*iter)->runTests();
            }
        }
    }

    namespace _internal {
        Module::Module(const char* name, std::function<void(Module*)> function) : moduleName(name) {
            Core::getInstance().addModule(this);
            function(this);
        }

        void Module::addTest(const char* name, std::function<void()> func) {
            tests[name] = func;
        }

        void Module::runTests() {
            for (auto iter = this->tests.begin(); iter != this->tests.end(); ++iter) {
                try {
                    iter->second();
                    Format::printPassedTest(iter->first);
                } catch (...) {
                    Format::printFailedTest(iter->first);
                }
            }
        }
    }

    // ----------------------------------------------------------------------------------------------------------------

    #define FOREMAN_CONCAT_(x,y)        x##y
    #define FOREMAN_CONCAT(x,y)         FOREMAN_CONCAT_(x,y)
    #define GENERATE_MODULE(number)     FOREMAN_CONCAT(static foreman::_internal::Module foremanInternalModule, __COUNTER__)
    #define Module(name, function)      GENERATE_MODULE(__COUNTER__)(name, [](foreman::_internal::Module* module) function )

    #define Test(name, function)        module->addTest(name, function)
}