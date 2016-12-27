#pragma once

#include <ostream>

#include <json.hpp>

#include "../result.hpp"

namespace dock {
	class JsonSerializer : public ResultSerializer {
    public:
        JsonSerializer(nlohmann::json& jsonObj, int indent = -1);

        virtual void            serialize(std::vector<Result>& results) override;
        virtual const char*     toCharArray() const override;
    private:
        nlohmann::json& outJson;
        int indent;

        static const char* resultArrayName;
        static const char* moduleName;
        static const char* testName;
        static const char* isPassedName;
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    const char* JsonSerializer::resultArrayName = u8"results";
    const char* JsonSerializer::moduleName = u8"module";
    const char* JsonSerializer::testName = u8"test";
    const char* JsonSerializer::isPassedName = u8"isPassed";

    JsonSerializer::JsonSerializer(nlohmann::json& jsonObj, int indent)
        : outJson(jsonObj), indent(indent) 
    {   }

    void JsonSerializer::serialize(std::vector<Result>& results) {
        outJson[resultArrayName] = nullptr;
        for (std::vector<Result>::size_type i = 0; i < results.size(); ++i) {
            outJson[resultArrayName][i] = nullptr;
            outJson[resultArrayName][i][moduleName] = results[i].module;
            outJson[resultArrayName][i][testName] = results[i].test;
            outJson[resultArrayName][i][isPassedName] = results[i].isPassed;
        }
    }

    const char* JsonSerializer::toCharArray() const {
        return this->outJson.dump(this->indent).c_str();
    }
}