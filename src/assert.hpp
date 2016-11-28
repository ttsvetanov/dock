#pragma once

#include <cstdint>
#include <cstddef>

#include <functional>
#include <exception>

namespace foreman {
	class AssertException : public std::exception {
	public:
		AssertException() : exception("Assert failed!") {}
	};

	class Assert {
	public:
		static void test(std::function<bool()> fcn);

		template<typename T>
		static void isEquals(const T a, const T b, std::function<bool(const T&, const T&)> compareFcn = nullptr);
	private:
		Assert() {}
	};

	// -------------------------------------------------------------------------------------------------------------------

	void Assert::test(std::function<bool()> fcn) {
		if (!fcn()) {
			throw AssertException();
		}
	}

	template<typename T>
	void Assert::isEquals(const T a, const T b, std::function<bool(const T&, const T&)> compareFcn) {
		if (compareFcn) {
			if (!compareFcn(a, b)) {
				throw AssertException();
			}
		} else {
			if (a != b) {
				throw AssertException();
			}
		}
	}

	template<>
	void Assert::isEquals<const char*>(const char* const a, const char* const b, std::function<bool(const char* const&, const char* const&)> compareFcn) {
		if (compareFcn) {
			if (!compareFcn(a, b)) {
				throw AssertException();
			}
		} else {
			if (strcmp(a, b)) {
				throw AssertException();
			}
		}
	}
}