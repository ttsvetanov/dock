#pragma once

#include <cstdint>
#include <cstddef>

#include <ostream>
#include <functional>
#include <vector>
#include <map>

#include "assert.hpp"
#include "format.hpp"
#include "result.hpp"

namespace foreman {
    class Core;

    namespace _internal {
        class Module {
        public:
            Module(const char* name, std::function<void(Module*)> function);

            const char* getName() const 
                { return this->moduleName; }

            size_t      getPassedTestsCount() const 
                { return this->passedTestsCount; }

            size_t      getTestsCount() const 
                { return this->tests.size(); }

            void        addTest(const char* name, std::function<void()> func);
            void        runTests(std::vector<Result>& resultsContainer);
        private:
            size_t      passedTestsCount;
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
        void putResultsIntoJson(nlohmann::json& jsonObject);
    private:
        static const size_t resizeStep = 100;
        static Core instance;

        Core();

        std::vector<_internal::Module*> modules;
        std::vector<Result> results;

        size_t  passedTestsCount;
        size_t  allTestsCount;
    };

    // ----------------------------------------------------------------------------------------------------------------

    Core Core::instance;

    // ----------------------------------------------------------------------------------------------------------------

    Core::Core()
        : passedTestsCount(0), allTestsCount(0) { }

    void Core::addModule(_internal::Module* module) {
        if (module == nullptr) {
            return;
        }

        try {
            if (this->modules.capacity() == this->modules.size()) {
                this->modules.reserve(this->modules.capacity() + Core::resizeStep);
            }
            this->modules.push_back(module);
            this->allTestsCount += module->getTestsCount();
        } catch (...) {
            Format::printAllocateError(module->getName());
            return;
        }
    }

    void Core::run() {
        this->passedTestsCount = 0;

        this->results.clear();
        this->results.reserve(this->allTestsCount);

        for (auto iter = this->modules.begin(); iter != this->modules.end(); ++iter) {
            if (*iter) {
                Format::printModuleName((*iter)->getName());
                (*iter)->runTests(this->results);
                this->passedTestsCount += (*iter)->getPassedTestsCount();
            }
        }
        Format::printStatistics(this->passedTestsCount, this->allTestsCount);
    }

    void Core::putResultsIntoJson(nlohmann::json& jsonObject) {
        JsonResultSerializer serializer(jsonObject);
        serializer.serialize(this->results);
        jsonObject[u8"testsPassed"] = this->passedTestsCount;
        jsonObject[u8"testsCount"] = this->allTestsCount;
    }

    // ----------------------------------------------------------------------------------------------------------------

    namespace _internal {
        Module::Module(const char* name, std::function<void(Module*)> function) 
            : passedTestsCount(0), moduleName(name) {
            function(this); // pre-collect data (for ex., tests cout)
            Core::getInstance().addModule(this);
        }

        void Module::addTest(const char* name, std::function<void()> func) {
            tests[name] = func;
        }

        void Module::runTests(std::vector<Result>& resultsContainer) {
            this->passedTestsCount = 0;
            for (auto iter = this->tests.begin(); iter != this->tests.end(); ++iter) {
                try {
                    iter->second();

                    this->passedTestsCount++;

                    if (resultsContainer.capacity() > resultsContainer.size()) {
                        resultsContainer.push_back(Result(this->getName(), (*iter).first, true));
                    }

                    Format::printPassedTest(iter->first);
                } catch (...) {
                    if (resultsContainer.capacity() > resultsContainer.size()) {
                        resultsContainer.push_back(Result(this->getName(), ( *iter ).first, false));
                    }

                    Format::printFailedTest(iter->first);
                }
            }
        }
    }

    // ----------------------------------------------------------------------------------------------------------------

    #define FOREMAN_CONCAT_(x,y)        x##y
    #define FOREMAN_CONCAT(x,y)         FOREMAN_CONCAT_(x,y)
    #define GENERATE_MODULE(number)     FOREMAN_CONCAT(static foreman::_internal::Module foremanInternalModule, __COUNTER__)
    #define Module(name, function)      GENERATE_MODULE(__COUNTER__)(name, [](foreman::_internal::Module* _foreman_module) function )

    #define Test(name, function)        _foreman_module->addTest(name, function)
}