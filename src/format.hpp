#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>

#include "./lib/termcolor.hpp"

namespace foreman {
    #if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    #endif
    
    class Encoding {
    public:
        Encoding();
        ~Encoding();
    private:
        UINT previousEncoding;
    };

    // ----------------------------------------------------------------------------------------------------------------

    class Format {
    public:
        static void enablePrint(bool en = true);
        static void printModuleName(const char* name);
        static void printPassedTest(const char* name);
        static void printFailedTest(const char* name);
        static void printAllocateError(const char* moduleName);
    private:
        Format() = default;

        static Format   instance;
        static bool     isOutputEnabled;  
    };

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------

    // for "invisible" change encoding table
    static Encoding foremanEncodingPatch;

    Format Format::instance;
    bool Format::isOutputEnabled = true;

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
        std::cout << u8"  " << name << u8"\n";
    }

    void Format::printPassedTest(const char* name) {
        if (Format::isOutputEnabled) {
            std::cout << termcolor::green << u8"    \u2713"
            //std::cout << termcolor::green << u8"    v"
                << termcolor::white << " " << name << ": "
                << termcolor::green << u8"пройден!\n"
                << termcolor::white;
        }
    }

    void Format::printFailedTest(const char* name) {
        if (Format::isOutputEnabled) {
            std::cout << termcolor::red << u8"    x"
                << termcolor::white << " " << name << ": "
                << termcolor::red << u8"провален!\n"
                << termcolor::reset;
        }
    }

    void Format::printAllocateError(const char* moduleName) {
        std::cout << termcolor::red << u8"    Error!"
            << termcolor::white << " Cann't allocate memory for modules container!\n"
            << "Module: " << moduleName << "\n"
            << termcolor::reset;
    }
}