#pragma once

#include <sstream>
#include <iomanip>

#include <iostream>

#include "../result.hpp"

namespace dock {
    class TextSerializer : public ResultSerializer {
    public:
        TextSerializer() = default;

        virtual void        serialize(std::vector<Result>& results) override;
        virtual std::string toString() const override;
    private:
        std::stringstream strstream;
    };

    // ----------------------------------------------------------------------------------------------------------------

    void TextSerializer::serialize(std::vector<Result>& results) {
        for (std::vector<Result>::size_type i = 0; i < results.size(); ++i) {
            strstream << "[" << results[i].module << "] " << results[i].test << ": ";
            if(results[i].isPassed) {
                strstream << u8"passed.";
            } else {
                strstream << u8"failed!";
            }
            if(i != (results.size() - 1)) {
                strstream << std::endl;
            }
        }
    }

    std::string TextSerializer::toString() const {
        return strstream.str();
    }
}