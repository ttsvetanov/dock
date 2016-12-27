#pragma once

#include <iostream>

#include "../result.hpp"

namespace dock {
	class StdOutSerializer : public ResultSerializer {
    public:
        StdOutSerializer(std::ostream& out);

        virtual void        serialize(std::vector<Result>& results) override;
    private:
        nlohmann::json& outJson;

        static const char* resultArrayName;
        static const char* moduleName;
        static const char* testName;
        static const char* isPassedName;
    };
}