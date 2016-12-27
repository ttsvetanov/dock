#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <iomanip>

#include <termcolor.hpp>

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

    class Format {
    public:
        static void enablePrint(bool en = true);
        static void printModuleName(const char* name);
        static void printPassedTest(const char* name);
        static void printFailedTest(const char* name);
        static void printAllocateError(const char* moduleName);
        static void printStatistics(const size_t passed, const size_t allTests);
    private:
        Format() = default;

        static Format   instance;
        static bool     isOutputEnabled;  

        static const char*  newLine;
        static const char*  passedWord;
        static const char*  passedSymbol;
        static const char*  failedSymbol;
        static const char*  failedWord;
        static const char*  errorWord;
        static const char*  allocatorErrorMessage;
        static const char*  moduleLineSpacer;
        static const char*  testLineSpacer;
        static const char*  statisticsPrefix;
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    // for "invisible" change encoding table
    static Encoding foremanEncodingPatch;

    Format Format::instance;
    bool Format::isOutputEnabled = true;

    const char* Format::newLine = u8"\n";
    const char* Format::passedWord = u8"passed!";
    const char* Format::passedSymbol = u8"\u2713";
    const char* Format::failedSymbol = u8"x";
    const char* Format::failedWord = u8"failed!";
    const char* Format::errorWord = u8"Error!";
    const char* Format::allocatorErrorMessage = u8"Cann't allocate memory for module: ";
    const char* Format::moduleLineSpacer = u8"  ";
    const char* Format::testLineSpacer = u8"    ";
    const char* Format::statisticsPrefix = u8"Statistics: ";

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

    // ----------------------------------------------------------------------------------------------------------------

    inline void Format::enablePrint(bool en) {
        Format::isOutputEnabled = en;
    }

    void Format::printModuleName(const char* name) {
        std::cout << Format::moduleLineSpacer << name << Format::newLine;
    }

    void Format::printPassedTest(const char* name) {
        if (Format::isOutputEnabled) {
            std::cout << termcolor::green << Format::testLineSpacer << Format::passedSymbol
                << termcolor::white << " " << name << ": "
                << termcolor::green << Format::passedWord << std::endl
                << termcolor::white;
        }
    }

    void Format::printFailedTest(const char* name) {
        if (Format::isOutputEnabled) {
            std::cout << termcolor::red << Format::testLineSpacer << Format::failedSymbol
                << termcolor::white << " " << name << ": "
                << termcolor::red << Format::failedWord << std::endl
                << termcolor::reset;
        }
    }

    void Format::printAllocateError(const char* moduleName) {
        std::cout << termcolor::red << Format::testLineSpacer << Format::errorWord
            << termcolor::white << u8" " << Format::allocatorErrorMessage << Format::newLine
            << moduleName << Format::newLine
            << termcolor::reset;
    }

    void Format::printStatistics(const size_t passed, const size_t allTests) {
        std::cout << Format::moduleLineSpacer; 

        float passedPercent = ( float(passed) / float(allTests) ) * 100.0f;
        if (passedPercent >= 80.0f) {
            std::cout << termcolor::green;
        } else if (passedPercent >= 30.0) {
            std::cout << termcolor::yellow;
        } else {
            std::cout << termcolor::red;
        }
        std::cout << Format::statisticsPrefix;

        std::cout << termcolor::green << passed
            << termcolor::white << u8"/"
            << termcolor::red << allTests - passed;

        if (passedPercent >= 80.0f) {
            std::cout << termcolor::green
                << u8" (" << std::fixed << std::setprecision(1) << passedPercent << u8"%)\n";
        } else if (passedPercent >= 30.0) {
            std::cout << termcolor::yellow
                << u8" (" << std::fixed << std::setprecision(1) << passedPercent << u8"%)\n";
        } else {
            std::cout << termcolor::red
                << u8" (" << std::fixed << std::setprecision(1) << passedPercent << u8"%)\n";
        }

        std::cout << termcolor::reset;
    }
}