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

    // ----------------------------------------------------------------------------------------------------------------

    class ResultSerializer {
    public:
        virtual void serialize(std::vector<Result>& results) = 0;
    };

    // ----------------------------------------------------------------------------------------------------------------

    class JsonResultSerializer : public ResultSerializer {
    public:
        JsonResultSerializer(nlohmann::json& jsonObj);

        virtual void        serialize(std::vector<Result>& results) override;
        nlohmann::json&     getSerializedJson() 
            { return this->outJson; }
    private:
        nlohmann::json& outJson;

        static const char* resultArrayName;
        static const char* moduleName;
        static const char* testName;
        static const char* isPassedName;
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    Result::Result(const char* _module, const char* _test, bool _isPassed) {
        this->module = _module;
        this->test = _test;
        this->isPassed = _isPassed;
    }

    // ----------------------------------------------------------------------------------------------------------------

    const char* JsonResultSerializer::resultArrayName = u8"results";
    const char* JsonResultSerializer::moduleName = u8"module";
    const char* JsonResultSerializer::testName = u8"test";
    const char* JsonResultSerializer::isPassedName = u8"isPassed";

    JsonResultSerializer::JsonResultSerializer(nlohmann::json& jsonObj)
        : outJson(jsonObj) 
    {   }

    void JsonResultSerializer::serialize(std::vector<Result>& results) {
        outJson[resultArrayName] = nullptr;
        for (std::vector<Result>::size_type i = 0; i < results.size(); ++i) {
            outJson[resultArrayName][i] = nullptr;
            outJson[resultArrayName][i][moduleName] = results[i].module;
            outJson[resultArrayName][i][testName] = results[i].test;
            outJson[resultArrayName][i][isPassedName] = results[i].isPassed;
        }
    }
}