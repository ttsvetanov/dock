#pragma once

#include <cstdint>
#include <cstddef>

#include <functional>

#include "assert.hpp"
#include "format.hpp"

#include "lib/json.hpp"

namespace foreman {
    class Result {
    public:
        Result(const char* module = u8"", const char* test = u8"", bool _isPassed = false);

        const char* module;
        const char* test;
        bool        isPassed;
    };

    Result::Result(const char* _module, const char* _test, bool _isPassed) {
        this->module = _module;
        this->test = _test;
        this->isPassed = _isPassed;
    }

    class ResultSerializer {
    public:
        //static void serialize();
    private:
        ResultSerializer() = delete;
    };

    // ----------------------------------------------------------------------------------------------------------------
}