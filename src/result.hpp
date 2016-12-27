#pragma once

#include <cstdint>
#include <cstddef>

#include <ostream>
#include <vector>
#include <functional>

#include "assert.hpp"

namespace dock {
    class Result {
    public:
        Result(const char* module = u8"", const char* test = u8"", bool _isPassed = false);

        const char* module;
        const char* test;
        bool        isPassed;
    };

    // ----------------------------------------------------------------------------------------------------------------

    class ResultSerializer {
    public:
        virtual void            serialize(std::vector<Result>& results) = 0;
        virtual const char*     toCharArray() const = 0;
        friend std::ostream&    operator<<(std::ostream& os, ResultSerializer& s);
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    Result::Result(const char* _module, const char* _test, bool _isPassed) {
        this->module = _module;
        this->test = _test;
        this->isPassed = _isPassed;
    }

    std::ostream& operator<<(std::ostream& os, ResultSerializer& s) {
        os << s.toCharArray();
        return os;
    }
}