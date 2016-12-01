#pragma once

#include <cstdint>
#include <cstddef>

#include <vector>
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

    class ResultSerializer {
    public:
        virtual void serialize(std::vector<Result>& results) = 0;
    };

    class JsonResultSerializer : public ResultSerializer {
    public:
        virtual void serialize(std::vector<Result>& results) override;
    };

    // ----------------------------------------------------------------------------------------------------------------

    Result::Result(const char* _module, const char* _test, bool _isPassed) {
        this->module = _module;
        this->test = _test;
        this->isPassed = _isPassed;
    }

    void JsonResultSerializer::serialize(std::vector<Result>& results) {
        using json = nlohmann::json;

        json outJson;

        outJson[u8"results"] = nullptr;
        for (auto iter = results.begin(); iter != results.end(); ++iter) {
            
        }
    }
}