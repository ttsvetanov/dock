#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

#include <string>
#include <functional>
#include <exception>

namespace dock {
    // Simple assert library for unit-testing engine
    class Assert {
    public:
        // General assert exception without specialized message 
        class Exception : public std::exception {
        public:
            Exception() : exception() {}

            virtual const char* what() const noexcept override {
                return u8"Assert failed";
            }
        };

        static void isTrue(const bool value);

        template<typename T>
        static void isEquals(const T a, const T b, std::function<bool(const T, const T)> compareFcn = defaultEqualsFunction<T>);

        template<typename T>
        static void isGreater(const T a, const T b, std::function<bool(const T, const T)> compareFcn = defaultGreaterFunction<T>);

        template<typename T>
        static void isLess(const T a, const T b, std::function<bool(const T, const T)> compareFcn = defaultLessFunction<T>);
    private:
        // Methods
        Assert() = default;

        template<typename T>
        static void generalAssertMethod(const T a, const T b, std::function<bool(const T, const T)> compareFcn);

        // Constants
        template<typename T>
        static const std::function<bool(const T, const T)> defaultEqualsFunction;

        template<typename T>
        static const std::function<bool(const T, const T)> defaultGreaterFunction;

        template<typename T>
        static const std::function<bool(const T, const T)> defaultLessFunction;
    };

    // -------------------------------------------------------------------------------------------------------------------

    // Definitions of static constants
    template<typename T>
    const std::function<bool(const T, const T)> Assert::defaultEqualsFunction =
        [](const T a, const T b) -> bool { return a == b; };

    template<typename T>
    const std::function<bool(const T, const T)> Assert::defaultGreaterFunction =
        [](const T a, const T b) -> bool { return a > b; };

    template<typename T>
    const std::function<bool(const T, const T)> Assert::defaultLessFunction =
        [](const T a, const T b) -> bool { return a < b; };

    // -------------------------------------------------------------------------------------------------------------------

    inline void Assert::isTrue(const bool value) {
        if (!value) {
            throw Assert::Exception();
        }
    }

    template<typename T>
    void Assert::isEquals(const T a, const T b, std::function<bool(const T, const T)> compareFcn) {
        Assert::generalAssertMethod(a, b, compareFcn);
    }

    template<>
    void Assert::isEquals<const char*>(const char* const a, const char* const b, 
                                    std::function<bool(const char* const, const char* const)> compareFcn) {
        if (compareFcn) {
            if (!compareFcn(a, b)) {
                throw Assert::Exception();
            }
        } else {
            std::size_t strALength = std::strlen(a);
            std::size_t strBLength = std::strlen(b);
            if (strALength != strBLength) {
                throw Assert::Exception();
            }
            if (std::strcmp(a, b)) {
                throw Assert::Exception();
            }
        }
    }

    // -------------------------------------------------------------------------------------------------------------------

    template<typename T>
    void Assert::isGreater(const T a, const T b, std::function<bool(const T, const T)> compareFcn) {
        Assert::generalAssertMethod(a, b, compareFcn);
    }

    // -------------------------------------------------------------------------------------------------------------------

    template<typename T>
    void Assert::isLess(const T a, const T b, std::function<bool(const T, const T)> compareFcn) {
        Assert::generalAssertMethod(a, b, compareFcn);
    }

    // -------------------------------------------------------------------------------------------------------------------

    template<typename T>
    void Assert::generalAssertMethod(const T a, const T b, std::function<bool(const T, const T)> compareFcn) {
        if (compareFcn) {
            if (!compareFcn(a, b)) {
                throw Assert::Exception();
            }
        } else {
            throw Assert::Exception();
        }
    }
}