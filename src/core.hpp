#pragma once

#include <cstdint>
#include <cstddef>

#include <ostream>
#include <iostream>
#include <functional>
#include <vector>
#include <map>

#include "assert.hpp"
#include "result.hpp"

namespace dock {
    class Core;

    namespace _internal {
        class Module {
        public:
            Module(const char* name, std::function<void(Module*)> function);

            const char* getName() const 
                { return this->moduleName; }

            size_t      getPassedCount() const 
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
        void addModule(_internal::Module* module);

        void run();
        void collect(ResultSerializer& serializer);

        size_t getPassedCount() const { return this->passedTestsCount; }
        size_t getTestsCount() const { return this->allTestsCount; }
        size_t getModulesCount() const { return this->modules.size(); }
    private:
        Core();
        friend Core& core();

        static const size_t resizeStep = 100;

        std::vector<_internal::Module*> modules;
        std::vector<Result> results;

        size_t  passedTestsCount;
        size_t  allTestsCount;
    };

    // ----------------------------------------------------------------------------------------------------------------

    Core& core() {
        static Core instance;
        return instance;
    }

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
            std::cout << "Can't allocate memory, line: " << __LINE__ << ", file: " << __FILE__ << std::endl; 
            return;
        }
    }

    void Core::run() {
        this->passedTestsCount = 0;

        this->results.clear();
        this->results.reserve(this->allTestsCount);

        for (auto iter = this->modules.begin(); iter != this->modules.end(); ++iter) {
            if (*iter) {
                (*iter)->runTests(this->results);
                this->passedTestsCount += (*iter)->getPassedCount();
            }
        }
    }

    void Core::collect(ResultSerializer& serializer) {
        serializer.serialize(this->results);
    }

    // ----------------------------------------------------------------------------------------------------------------

    namespace _internal {
        Module::Module(const char* name, std::function<void(Module*)> function) 
            : passedTestsCount(0), moduleName(name) {
            function(this); // pre-collect data (for ex., tests cout)
            core().addModule(this);
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
                } catch (...) {
                    if (resultsContainer.capacity() > resultsContainer.size()) {
                        resultsContainer.push_back(Result(this->getName(), ( *iter ).first, false));
                    }
                }
            }
        }
    }

    // ----------------------------------------------------------------------------------------------------------------

    #define DOCK_CONCAT_(x,y)           x##y
    #define DOCK_CONCAT(x,y)            DOCK_CONCAT_(x,y)
    #define GENERATE_MODULE(number)     DOCK_CONCAT(static dock::_internal::Module dockInternalModule, __COUNTER__)
    #define Module(name, function)      GENERATE_MODULE(__COUNTER__)(name, [](dock::_internal::Module* _dock_module) function )

    #define Test(name, function)        _dock_module->addTest(name, function)
}