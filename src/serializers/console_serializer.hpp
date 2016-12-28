#pragma once

#include <iostream>

#include <termcolor.hpp>

#include "../result.hpp"

namespace dock {
    #if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    #endif
    
    class Encoding {
    public:
        Encoding();
        ~Encoding();
    private:
        #if defined(_WIN32) || defined(_WIN64)
        UINT previousEncoding;
        #endif
    };

    // ----------------------------------------------------------------------------------------------------------------

    class ConsoleSerializer : public ResultSerializer {
    public:
        ConsoleSerializer() = default;

        virtual void        serialize(std::vector<Result>& results) override;
        virtual std::string toString() const override;

        friend std::ostream& operator<<(std::ostream& os, ConsoleSerializer& s);
    private:
        std::vector<Result>* results;

        static const char*  newLine;
        static const char*  passedWord;
        static const char*  passedSymbol;
        static const char*  failedSymbol;
        static const char*  failedWord;
        static const char*  errorWord;
        static const char*  moduleLineSpacer;
        static const char*  testLineSpacer;
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    Encoding::Encoding() {
        #if defined(_WIN32) || defined(_WIN64)
        this->previousEncoding = GetConsoleOutputCP();
        SetConsoleOutputCP(65001);
        #endif
    }

    Encoding::~Encoding() {
        #if defined(_WIN32) || defined(_WIN64)
        SetConsoleOutputCP(this->previousEncoding);
        #endif
    }

    static Encoding dockEncodingPatch;

    // ----------------------------------------------------------------------------------------------------------------

    const char* ConsoleSerializer::newLine = u8"\n";
    const char* ConsoleSerializer::passedWord = u8"passed!";
    const char* ConsoleSerializer::passedSymbol = u8"\u2713";
    const char* ConsoleSerializer::failedSymbol = u8"x";
    const char* ConsoleSerializer::failedWord = u8"failed!";
    const char* ConsoleSerializer::errorWord = u8"Error!";
    const char* ConsoleSerializer::moduleLineSpacer = u8"  ";
    const char* ConsoleSerializer::testLineSpacer = u8"    ";

    void ConsoleSerializer::serialize(std::vector<Result>& results) {
        this->results = &results;
    }

    std::string ConsoleSerializer::toString() const {
        return std::string();
    }

    std::ostream& operator<<(std::ostream& os, ConsoleSerializer& s) {
        if(s.results) {
            std::vector<Result>& res = *(s.results);
            const char* lastModuleStringPtr = nullptr;
            for(std::vector<Result>::size_type i = 0; i < res.size(); ++i) {
                if(lastModuleStringPtr != res[i].module) {
                    lastModuleStringPtr = res[i].module;
                    os << ConsoleSerializer::moduleLineSpacer << res[i].module << ConsoleSerializer::newLine;
                }

                if(res[i].isPassed) {
                    os << termcolor::green << ConsoleSerializer::testLineSpacer << ConsoleSerializer::passedSymbol
                        << termcolor::white << " " << res[i].test << ": "
                        << termcolor::green << ConsoleSerializer::passedWord << ConsoleSerializer::newLine
                        << termcolor::white;
                } else {
                    os << termcolor::red << ConsoleSerializer::testLineSpacer << ConsoleSerializer::failedSymbol
                        << termcolor::white << " " << res[i].test << ": "
                        << termcolor::red << ConsoleSerializer::failedWord << ConsoleSerializer::newLine
                        << termcolor::reset;
                }
            }
        }

        return os;
    }
}